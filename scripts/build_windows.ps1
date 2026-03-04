<#
.SYNOPSIS
    Windows build orchestration script for the Tiger OpenAPI C++ SDK.

.DESCRIPTION
    This script codifies the Windows guidance from README.md:
      * Requires a Visual Studio C++ workload and CMake 3.15+.
      * Installs OpenSSL and cpprestsdk with vcpkg using the selected triplet.
      * Builds Boost 1.86.0 and Protobuf 3.21.12 directly from their official source archives
        (matching the Linux/macOS instructions) and caches the artifacts under .deps/.
      * Configures, builds, installs, and optionally runs the SDK demo using the freshly built
        dependencies so that Windows behavior mirrors Linux/macOS.

    Typical usage:
        powershell -ExecutionPolicy Bypass -File scripts/build_windows.ps1

    Advanced options allow skipping dependency builds, reusing system-provided Boost/Protobuf,
    or pointing at custom install prefixes.

.NOTES
    The script intentionally favors clarity over a huge build matrix. Invoke it once per triplet
    or runtime flavor when you need additional variants.
#>
[CmdletBinding()]param(
    [ValidateSet("x64-windows","x86-windows","arm64-windows","x64-windows-static","x86-windows-static","arm64-windows-static")]
    [string]$Triplet = "x64-windows",

    [ValidateSet("Release","Debug")]
    [string]$BuildType = "Release",

    [ValidateSet("MD","MT")]
    [string]$Runtime = "MD",

    [string]$Generator,
    [string]$VcpkgRoot,
    [switch]$UpdateVcpkg,
    [switch]$ForceBootstrap,

    [string]$InstallPrefix,
    [string]$BuildDir,
    [string]$DemoBuildDir,

    [switch]$SkipDeps,
    [switch]$SkipSdk,
    [switch]$SkipInstall,
    [switch]$SkipDemo,
    [switch]$RunDemo,

    [string]$BoostRoot,
    [string]$ProtobufRoot,

    [string]$BoostVersion = "1.86.0",
    [string]$ProtobufVersion = "3.21.12"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

if (-not $PSScriptRoot) {
    $PSScriptRoot = Split-Path -Parent $MyInvocation.MyCommand.Path
}
$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot ".." ) ).Path

if (-not $BuildDir) {
    $BuildDir = Join-Path $RepoRoot "build/windows"
}
if (-not $DemoBuildDir) {
    $DemoBuildDir = Join-Path $RepoRoot "demo/build/windows"
}
if (-not $InstallPrefix) {
    $InstallPrefix = Join-Path $RepoRoot "output/Windows/tigerapi-sdk"
}
$InstallPrefix = $InstallPrefix.TrimEnd('\\/').Trim()
if (-not $InstallPrefix) {
    throw "InstallPrefix cannot be empty."
}

if (-not $VcpkgRoot) {
    $VcpkgRoot = Join-Path $RepoRoot ".vcpkg"
}

$DepsRoot          = Join-Path $RepoRoot ".deps"
$DownloadsDir      = Join-Path $DepsRoot "downloads"
$SourceDir         = Join-Path $DepsRoot "src"
$DepsBuildDir      = Join-Path $DepsRoot "build"
$DepsInstallDir    = Join-Path $DepsRoot "install"
$ProcessorCount    = [Math]::Max([Environment]::ProcessorCount, 1)
$CmakeDefaultGenerator = "Visual Studio 17 2022"
$PortableCmakeVersion  = "3.28.3"
if (-not $Generator) {
    $Generator = $CmakeDefaultGenerator
}
$VcpkgManifestPath = Join-Path $RepoRoot "vcpkg.json"
$HasVcpkgManifest = Test-Path $VcpkgManifestPath

function Write-Info { param([string]$Message) Write-Host "[+] $Message" -ForegroundColor Green }
function Write-Warn { param([string]$Message) Write-Host "[!] $Message" -ForegroundColor Yellow }
function Write-Detail { param([string]$Message) Write-Host "[ ] $Message" -ForegroundColor Cyan }

function Require-Command {
    param(
        [Parameter(Mandatory)][string]$Command,
        [string]$FriendlyName,
        [string]$WingetId
    )

    if (Get-Command $Command -ErrorAction SilentlyContinue) {
        return
    }

    $name = if ($FriendlyName) { $FriendlyName } else { $Command }
    $winget = Get-Command winget -ErrorAction SilentlyContinue
    if (-not $WingetId -or -not $winget) {
        throw "Required tool '$name' is missing and winget is unavailable. Install it manually and rerun."
    }

    Write-Warn "Attempting to install $name via winget ($WingetId)"
    winget install --id $WingetId --exact --silent --accept-source-agreements --accept-package-agreements | Out-Null
    if (-not (Get-Command $Command -ErrorAction SilentlyContinue)) {
        throw "Unable to locate '$name' after installation attempt."
    }
}

function Ensure-Directory {
    param([Parameter(Mandatory)][string]$Path)
    if (-not (Test-Path $Path)) {
        New-Item -ItemType Directory -Path $Path -Force | Out-Null
    }
    return (Resolve-Path $Path).Path
}

$null = Ensure-Directory -Path $DepsRoot
$null = Ensure-Directory -Path $DownloadsDir
$null = Ensure-Directory -Path $SourceDir
$null = Ensure-Directory -Path $DepsBuildDir
$null = Ensure-Directory -Path $DepsInstallDir

function Test-ZipArchive {
    param([Parameter(Mandatory)][string]$ArchivePath)
    if (-not (Test-Path $ArchivePath)) { return $false }
    try {
        $stream = [System.IO.File]::Open($ArchivePath, [System.IO.FileMode]::Open, [System.IO.FileAccess]::Read, [System.IO.FileShare]::Read)
        try {
            $buffer = New-Object byte[] 4
            $read = $stream.Read($buffer, 0, 4)
            return ($read -eq 4 -and $buffer[0] -eq 0x50 -and $buffer[1] -eq 0x4B)
        }
        finally {
            $stream.Dispose()
        }
    }
    catch {
        return $false
    }
}

function Download-Archive {
    param(
        [string[]]$Urls,
        [string]$Destination,
        [string]$FriendlyName
    )

    if (-not $Urls -or $Urls.Count -eq 0) {
        throw "No URLs provided to Download-Archive for $FriendlyName"
    }

    foreach ($url in $Urls) {
        $attempt = 0
        $maxAttempts = 3
        while ($attempt -lt $maxAttempts) {
            $attempt++
            try {
                if (Test-Path $Destination) {
                    Remove-Item -Path $Destination -Force -ErrorAction SilentlyContinue
                }
                Write-Info "Downloading $FriendlyName from $url (attempt $attempt/$maxAttempts)"
                Invoke-WebRequest -Uri $url -OutFile $Destination -UseBasicParsing
            }
            catch {
                Write-Warn "Download attempt $attempt from $url failed: $($_.Exception.Message)"
                Start-Sleep -Seconds (2 * $attempt)
                continue
            }

            if (Test-ZipArchive -ArchivePath $Destination) {
                Write-Info "Downloaded and validated $FriendlyName"
                return (Resolve-Path $Destination).Path
            }

            Write-Warn "Downloaded file from $url is invalid (archive header mismatch)."
            Remove-Item -Path $Destination -Force -ErrorAction SilentlyContinue
            Start-Sleep -Seconds 1
        }
        Write-Warn "All attempts from $url failed; trying next URL if available."
    }

    throw "Unable to download a valid archive for $FriendlyName from the supplied URLs."
}

function Extract-Archive {
    param(
        [Parameter(Mandatory)][string]$ArchivePath,
        [Parameter(Mandatory)][string]$DestinationPath,
        [string[]]$RetryUrls,
        [string]$FriendlyName
    )

    $FriendlyName = if ($FriendlyName) { $FriendlyName } else { (Split-Path $ArchivePath -Leaf) }

    # Ensure destination is clean
    if (Test-Path $DestinationPath) {
        Remove-Item -Path $DestinationPath -Recurse -Force -ErrorAction SilentlyContinue
    }

    try {
        Write-Info "Extracting $FriendlyName"
        Expand-Archive -LiteralPath $ArchivePath -DestinationPath $DestinationPath -Force
        return
    }
    catch {
        Write-Warn "Initial extraction of $FriendlyName failed: $($_.Exception.Message)"
        # If archive appears corrupted, try to re-download (if URLs provided) and retry once.
        $isZip = Test-ZipArchive -ArchivePath $ArchivePath
        if (-not $isZip) {
            Write-Warn "Archive header invalid for $ArchivePath. Removing and attempting re-download."
            Remove-Item -Path $ArchivePath -Force -ErrorAction SilentlyContinue
            if ($RetryUrls -and $RetryUrls.Count -gt 0) {
                Download-Archive -Urls $RetryUrls -Destination $ArchivePath -FriendlyName $FriendlyName
            } else {
                throw "Archive $ArchivePath is invalid and no retry URLs were provided."
            }
        }

        try {
            Write-Info "Retrying extraction of $FriendlyName"
            Expand-Archive -LiteralPath $ArchivePath -DestinationPath $DestinationPath -Force
            return
        }
        catch {
            Write-Warn "Retry extraction of $FriendlyName still failed: $($_.Exception.Message)"
            throw "Failed to extract $FriendlyName from $ArchivePath. Try removing the file and rerunning the script."
        }
    }
}

function Ensure-PortableCMake {
    if (Get-Command cmake -ErrorAction SilentlyContinue) {
        return
    }

    $cmakeFolder = "cmake-$PortableCmakeVersion-windows-x86_64"
    $cmakeRoot = Join-Path $DepsRoot $cmakeFolder
    $cmakeExe = Join-Path $cmakeRoot "bin/cmake.exe"
    if (-not (Test-Path $cmakeExe)) {
        $archiveName = "$cmakeFolder.zip"
        $archivePath = Join-Path $DownloadsDir $archiveName
        $url = "https://github.com/Kitware/CMake/releases/download/v$PortableCmakeVersion/$archiveName"
        if (-not (Test-Path $archivePath)) {
            Write-Info "Downloading portable CMake $PortableCmakeVersion"
            Invoke-WebRequest -Uri $url -OutFile $archivePath -UseBasicParsing
        }
        if (Test-Path $cmakeRoot) {
            Remove-Item -Path $cmakeRoot -Recurse -Force
        }
        Write-Info "Extracting portable CMake"
        Extract-Archive -ArchivePath $archivePath -DestinationPath $DepsRoot -RetryUrls @($url) -FriendlyName "CMake $PortableCmakeVersion"
    }

    if (Test-Path $cmakeExe) {
        $cmakeBin = Split-Path $cmakeExe -Parent
        if (-not ($env:PATH -split ';' | Where-Object { $_ -eq $cmakeBin })) {
            $env:PATH = "$cmakeBin;$env:PATH"
            Write-Info "Added portable CMake $PortableCmakeVersion to PATH"
        }
    }
}

function Ensure-VcpkgRepository {
    if (-not (Test-Path $VcpkgRoot)) {
        Write-Info "Cloning vcpkg -> $VcpkgRoot"
        git clone https://github.com/microsoft/vcpkg $VcpkgRoot | Out-Null
        return
    }

    if ($UpdateVcpkg) {
        Write-Info "Updating vcpkg checkout"
        Push-Location $VcpkgRoot
        try {
            git fetch --all --prune | Out-Null
            git pull --ff-only | Out-Null
        }
        finally {
            Pop-Location
        }
    }
}

function Bootstrap-Vcpkg {
    $vcpkgExe = Join-Path $VcpkgRoot "vcpkg.exe"
    if ($ForceBootstrap -or -not (Test-Path $vcpkgExe)) {
        Write-Info "Bootstrapping vcpkg"
        & (Join-Path $VcpkgRoot "bootstrap-vcpkg.bat") -disableMetrics | Out-Null
    }
    if (-not (Test-Path $vcpkgExe)) {
        throw "vcpkg.exe not found after bootstrap."
    }
    return (Resolve-Path $vcpkgExe).Path
}

function Install-VcpkgPackages {
    param(
        [string]$VcpkgExe,
        [string]$TripletValue,
        [string[]]$Packages
    )

    if ($HasVcpkgManifest) {
        Push-Location $RepoRoot
        try {
            $args = @("install")
            if ($TripletValue) { $args += "--triplet"; $args += $TripletValue }
            & $VcpkgExe @args
            if ($LASTEXITCODE -ne 0) {
                throw "vcpkg install failed for manifest triplet '$TripletValue'"
            }
        }
        finally {
            Pop-Location
        }
        return
    }

    if (-not $Packages -or $Packages.Count -eq 0) { return }
    $args = @("install")
    foreach ($pkg in $Packages) {
        if ($TripletValue) {
            $args += "${pkg}:$TripletValue"
        } else {
            $args += $pkg
        }
    }
    $args += "--clean-after-build"
    & $VcpkgExe @args
    if ($LASTEXITCODE -ne 0) {
        throw "vcpkg install failed for packages $($Packages -join ', ')"
    }
}

function Get-TripletDescriptor {
    param([string]$TripletValue)
    if ($TripletValue -match "x64") { return [pscustomobject]@{ ArchArg = "x64";   ArchLabel = "x64" } }
    if ($TripletValue -match "x86") { return [pscustomobject]@{ ArchArg = "Win32"; ArchLabel = "Win32" } }
    if ($TripletValue -match "arm64") { return [pscustomobject]@{ ArchArg = "ARM64"; ArchLabel = "ARM64" } }
    return $null
}

function Get-MsvcRuntimeLibrary {
    param([string]$RuntimeFlavor, [string]$Configuration)
    $runtimeValue = if ([string]::IsNullOrWhiteSpace($RuntimeFlavor)) { "MD" } else { $RuntimeFlavor }
    $runtime = $runtimeValue.ToUpperInvariant()
    $isDebug = $Configuration -like "*Debug*"
    switch ($runtime) {
        "MT" {
            if ($isDebug) { return "MultiThreadedDebug" }
            return "MultiThreaded"
        }
        default {
            if ($isDebug) { return "MultiThreadedDebugDLL" }
            return "MultiThreadedDLL"
        }
    }
}

function Prepare-BoostDependency {
    param(
        [string]$Version,
        [pscustomobject]$Descriptor
    )

    if ($BoostRoot) {
        $resolved = Resolve-Path $BoostRoot -ErrorAction Stop
        $includeDir = Join-Path $resolved "include"
        $libDir = Join-Path $resolved "lib"
        if (-not (Test-Path $includeDir) -or -not (Test-Path $libDir)) {
            throw "Provided Boost root '$resolved' is missing include/ or lib/."
        }
        return [pscustomobject]@{ Root=$resolved; Include=$includeDir; Lib=$libDir }
    }

    if ($SkipDeps) {
        throw "Boost root not provided and -SkipDeps was supplied."
    }

    $token = $Version -replace '\.', '_'
    $archiveName = "boost_${token}.zip"
    $archivePath = Join-Path $DownloadsDir $archiveName
    $urls = @(
        "https://archives.boost.io/release/$Version/source/$archiveName",
        "https://boostorg.jfrog.io/artifactory/main/release/$Version/source/$archiveName"
    )
    if (-not (Test-Path $archivePath)) {
        $null = Download-Archive -Urls $urls -Destination $archivePath -FriendlyName "Boost $Version"
    } elseif (-not (Test-ZipArchive -ArchivePath $archivePath)) {
        Remove-Item -Path $archivePath -Force
        $null = Download-Archive -Urls $urls -Destination $archivePath -FriendlyName "Boost $Version"
    }

    $sourceDir = Join-Path $SourceDir "boost_${token}"
    $hasFiles = $false
    if (Test-Path $sourceDir) {
        $hasFiles = (Get-ChildItem -Path $sourceDir -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1) -ne $null
    }
    if (-not $hasFiles) {
        Write-Info "Extracting Boost $Version"
        Extract-Archive -ArchivePath $archivePath -DestinationPath $SourceDir -RetryUrls $urls -FriendlyName "Boost $Version"
    }

    $tripletRoot = Ensure-Directory -Path (Join-Path $DepsInstallDir $Triplet)
    $installDir = Join-Path $tripletRoot "boost-$Version"
    $stamp = Join-Path $installDir ".complete"
    if (-not (Test-Path $stamp)) {
        Ensure-Directory -Path $installDir | Out-Null
        $addressModel = 64
        $architecture = "x86"
        switch ($Descriptor.ArchArg) {
            "Win32" { $addressModel = 32; $architecture = "x86" }
            "ARM64" { $addressModel = 64; $architecture = "arm" }
        }

        $libraries = @("chrono","filesystem","log","program_options","system","thread")
        $libraryArgs = $libraries | ForEach-Object { "--with-$_" }

        Push-Location $sourceDir
        try {
            Write-Info "Bootstrapping Boost build system"
            & (Join-Path $sourceDir "bootstrap.bat") | Out-Null
            $b2 = Join-Path $sourceDir "b2.exe"
            if (-not (Test-Path $b2)) { throw "b2.exe not found after bootstrap." }

            $b2Args = @(
                "toolset=msvc",
                "address-model=$addressModel",
                "architecture=$architecture",
                "link=shared,static",
                "runtime-link=shared,static",
                "threading=multi",
                "variant=debug,release",
                "--layout=tagged",
                "--prefix=$installDir",
                "-j$ProcessorCount"
            ) + $libraryArgs + "install"

            Write-Info "Building Boost $Version for $Triplet"
            & $b2 @b2Args | Out-Null
        }
        finally {
            Pop-Location
        }
        New-Item -ItemType File -Path $stamp -Force | Out-Null
    }

    return [pscustomobject]@{
        Root    = (Resolve-Path $installDir).Path
        Include = Join-Path $installDir "include"
        Lib     = Join-Path $installDir "lib"
    }
}

function Prepare-ProtobufDependency {
    param(
        [string]$Version,
        [pscustomobject]$Descriptor
    )

    if ($ProtobufRoot) {
        $resolved = Resolve-Path $ProtobufRoot -ErrorAction Stop
        $includeDir = Join-Path $resolved "include"
        $libDir = Join-Path $resolved "lib"
        $binDir = Join-Path $resolved "bin"
        $protoc = Join-Path $binDir "protoc.exe"
        foreach ($path in @($includeDir,$libDir,$binDir,$protoc)) {
            if (-not (Test-Path $path)) {
                throw "Provided Protobuf root '$resolved' is missing $path."
            }
        }
        return [pscustomobject]@{ Root=$resolved; Include=$includeDir; Lib=$libDir; Bin=$binDir; Protoc=$protoc }
    }

    if ($SkipDeps) {
        throw "Protobuf root not provided and -SkipDeps was supplied."
    }

    $archiveName = "protobuf-cpp-$Version.zip"
    $archivePath = Join-Path $DownloadsDir $archiveName
    if (-not (Test-Path $archivePath)) {
        $null = Download-Archive -Urls @("https://github.com/protocolbuffers/protobuf/releases/download/v$Version/$archiveName") -Destination $archivePath -FriendlyName "Protobuf $Version"
    } elseif (-not (Test-ZipArchive -ArchivePath $archivePath)) {
        Remove-Item -Path $archivePath -Force
        $null = Download-Archive -Urls @("https://github.com/protocolbuffers/protobuf/releases/download/v$Version/$archiveName") -Destination $archivePath -FriendlyName "Protobuf $Version"
    }

    $sourceDir = Join-Path $SourceDir "protobuf-$Version"
    $hasFiles = $false
    if (Test-Path $sourceDir) {
        $hasFiles = (Get-ChildItem -Path $sourceDir -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1) -ne $null
    }
    if (-not $hasFiles) {
        Write-Info "Extracting Protobuf $Version"
        Extract-Archive -ArchivePath $archivePath -DestinationPath $SourceDir -RetryUrls @("https://github.com/protocolbuffers/protobuf/releases/download/v$Version/$archiveName") -FriendlyName "Protobuf $Version"
    }

    $tripletRoot = Ensure-Directory -Path (Join-Path $DepsInstallDir $Triplet)
    $runtimeToken = $Runtime.ToUpperInvariant()
    $installDir = Join-Path $tripletRoot "protobuf-$Version-$runtimeToken"
    $stamp = Join-Path $installDir ".complete"

    if (-not (Test-Path $stamp)) {
        if (Test-Path $installDir) {
            Remove-Item -Path $installDir -Recurse -Force
        }
        Ensure-Directory -Path $installDir | Out-Null

        $buildDir = Join-Path $DepsBuildDir "protobuf-$Triplet-$runtimeToken"
        if (Test-Path $buildDir) {
            Remove-Item -Path $buildDir -Recurse -Force
        }
        Ensure-Directory -Path $buildDir | Out-Null

        $runtimeLibrary = Get-MsvcRuntimeLibrary -RuntimeFlavor $Runtime -Configuration $BuildType
        $staticRuntime = if ($Runtime.ToUpperInvariant() -eq "MT") { "ON" } else { "OFF" }

        $cmakeArgs = @("-S", $sourceDir, "-B", $buildDir,
            "-DCMAKE_INSTALL_PREFIX=$installDir",
            "-Dprotobuf_BUILD_TESTS=OFF",
            "-Dprotobuf_BUILD_CONFORMANCE=OFF",
            "-Dprotobuf_WITH_ZLIB=ON",
            "-Dprotobuf_BUILD_SHARED_LIBS=ON",
            "-Dprotobuf_MSVC_STATIC_RUNTIME=$staticRuntime",
            "-DCMAKE_MSVC_RUNTIME_LIBRARY=$runtimeLibrary"
        )
        if ($Generator) { $cmakeArgs = @("-G", $Generator) + $cmakeArgs }
        if ($Descriptor -and $Descriptor.ArchArg -and ($Generator -match "Visual Studio")) {
            $cmakeArgs = @("-A", $Descriptor.ArchArg) + $cmakeArgs
        }

        Write-Info "Configuring Protobuf $Version"
        cmake @cmakeArgs

        foreach ($config in @("Release","Debug")) {
            $buildArgs = @("--build", $buildDir, "--config", $config, "--target", "install", "--parallel", $ProcessorCount)
            cmake @buildArgs
        }

        New-Item -ItemType File -Path $stamp -Force | Out-Null
    }

    $root = (Resolve-Path $installDir).Path
    $includeDir = Join-Path $root "include"
    $libDir = Join-Path $root "lib"
    $binDir = Join-Path $root "bin"
    $protoc = Join-Path $binDir "protoc.exe"
    foreach ($path in @($includeDir,$libDir,$binDir,$protoc)) {
        if (-not (Test-Path $path)) {
            throw "Missing Protobuf component '$path' after build."
        }
    }

    return [pscustomobject]@{ Root=$root; Include=$includeDir; Lib=$libDir; Bin=$binDir; Protoc=$protoc }
}

function Invoke-CMakeConfigure {
    param(
        [string]$SourceDir,
        [string]$BinaryDir,
        [string]$BuildTypeValue,
        [string]$GeneratorValue,
        [string]$Architecture,
        [string]$ToolchainFile,
        [hashtable]$Definitions
    )

    Ensure-Directory -Path $BinaryDir | Out-Null
    $args = @("-S", $SourceDir, "-B", $BinaryDir, "-DCMAKE_BUILD_TYPE=$BuildTypeValue")
    if ($GeneratorValue) { $args = @("-G", $GeneratorValue) + $args }
    if ($Architecture) { $args = @("-A", $Architecture) + $args }
    if ($ToolchainFile) { $args += "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile" }
    foreach ($key in $Definitions.Keys) {
        $value = $Definitions[$key]
        if ($null -ne $value -and $value -ne "") {
            $args += "-D${key}=$value"
        }
    }
    cmake @args
}

function Invoke-CMakeBuild {
    param([string]$BinaryDir,[string]$BuildTypeValue,[string[]]$Targets)
    $args = @("--build", $BinaryDir, "--config", $BuildTypeValue, "--parallel", $ProcessorCount)
    if ($Targets -and $Targets.Count -gt 0) { $args += "--target"; $args += $Targets }
    cmake @args
}

function Invoke-CMakeInstall { param([string]$BinaryDir,[string]$BuildTypeValue) cmake --install $BinaryDir --config $BuildTypeValue }

function Resolve-TigerApiLibrary {
    param([string]$InstallDir,[string]$BuildDir,[string]$Config)
    $searchDirs = @(
        (Join-Path $InstallDir "lib"),
        (Join-Path $InstallDir "bin"),
        $BuildDir,
        (Join-Path $BuildDir $Config)
    )
    foreach ($dir in $searchDirs) {
        if (-not (Test-Path $dir)) { continue }
        $lib = Get-ChildItem -Path $dir -Filter "tigerapi*.lib" -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($lib) { return $lib.FullName }
        $dll = Get-ChildItem -Path $dir -Filter "tigerapi*.dll" -File -ErrorAction SilentlyContinue | Select-Object -First 1
        if ($dll) { return $dll.FullName }
    }
    return $null
}

function Resolve-DemoExecutable {
    param([string]$BuildDir,[string]$Config)
    $candidates = @(
        (Join-Path $BuildDir "openapi_cpp_test.exe"),
        (Join-Path (Join-Path $BuildDir $Config) "openapi_cpp_test.exe")
    )
    foreach ($exe in $candidates) {
        if (Test-Path $exe) { return $exe }
    }
    return $null
}

function Invoke-DemoExecutable {
    param([string]$Executable,[string[]]$RuntimePaths)
    if (-not (Test-Path $Executable)) { throw "Demo executable not found at $Executable" }
    $valid = @()
    foreach ($path in $RuntimePaths) {
        if ([string]::IsNullOrWhiteSpace($path)) { continue }
        if (-not (Test-Path $path)) { continue }
        $resolved = (Resolve-Path $path).Path
        if (-not ($valid -contains $resolved)) { $valid += $resolved }
    }
    $originalPath = $env:PATH
    if ($valid.Count -gt 0) {
        $env:PATH = ($valid -join ';') + ';' + $env:PATH
    }
    try {
        & $Executable
    }
    finally {
        $env:PATH = $originalPath
    }
}

Write-Info "Validating prerequisites"
Ensure-PortableCMake
Require-Command -Command git -FriendlyName "Git" -WingetId "Git.Git"
Require-Command -Command cmake -FriendlyName "CMake" -WingetId "Kitware.CMake"
Require-Command -Command powershell -FriendlyName "PowerShell" # always present but keeps parity

$descriptor = Get-TripletDescriptor -TripletValue $Triplet
if (-not $descriptor) {
    throw "Unable to derive architecture from triplet '$Triplet'."
}

Ensure-VcpkgRepository
$vcpkgExe = Bootstrap-Vcpkg
Write-Info "Installing vcpkg dependencies"
Install-VcpkgPackages -VcpkgExe $vcpkgExe -TripletValue $Triplet -Packages @("openssl","cpprestsdk")
Write-Info "Finished vcpkg dependency installation"

$env:VCPKG_ROOT = $VcpkgRoot
$env:VCPKG_DEFAULT_TRIPLET = $Triplet
$toolchainFile = Join-Path $VcpkgRoot "scripts/buildsystems/vcpkg.cmake"
if (-not (Test-Path $toolchainFile)) {
    throw "vcpkg toolchain file missing at $toolchainFile"
}

Write-Info "Preparing Boost/Protobuf source dependencies"
$boostLayout = Prepare-BoostDependency -Version $BoostVersion -Descriptor $descriptor
$protobufLayout = Prepare-ProtobufDependency -Version $ProtobufVersion -Descriptor $descriptor
$tripletRoot = Join-Path $VcpkgRoot "installed" $Triplet

if (-not $SkipSdk) {
    Write-Info "Configuring Tiger OpenAPI SDK"
    $sdkDefines = @{
        "CMAKE_INSTALL_PREFIX"       = $InstallPrefix
        "CMAKE_MSVC_RUNTIME_LIBRARY" = Get-MsvcRuntimeLibrary -RuntimeFlavor $Runtime -Configuration $BuildType
        "BOOST_ROOT"                 = $boostLayout.Root
        "BOOST_INCLUDEDIR"           = $boostLayout.Include
        "BOOST_LIBRARYDIR"           = $boostLayout.Lib
        "Boost_USE_STATIC_LIBS"      = "ON"
        "Protobuf_ROOT"              = $protobufLayout.Root
        "Protobuf_DIR"               = Join-Path $protobufLayout.Root "lib/cmake/protobuf"
        "Protobuf_PROTOC_EXECUTABLE" = $protobufLayout.Protoc
        "Protobuf_USE_STATIC_LIBS"   = "OFF"
        "CPPREST_PREFIX"             = $tripletRoot
        "OPENSSL_ROOT_DIR"           = $tripletRoot
        "CMAKE_PREFIX_PATH"          = ($boostLayout.Root, $protobufLayout.Root, $tripletRoot -join ';')
        "VCPKG_TARGET_TRIPLET"       = $Triplet
    }
    Invoke-CMakeConfigure -SourceDir $RepoRoot -BinaryDir $BuildDir -BuildTypeValue $BuildType -GeneratorValue $Generator -Architecture $descriptor.ArchArg -ToolchainFile $toolchainFile -Definitions $sdkDefines
    Write-Info "Building Tiger OpenAPI SDK"
    Invoke-CMakeBuild -BinaryDir $BuildDir -BuildTypeValue $BuildType -Targets @()
    if (-not $SkipInstall) {
        Write-Info "Installing Tiger OpenAPI SDK -> $InstallPrefix"
        Invoke-CMakeInstall -BinaryDir $BuildDir -BuildTypeValue $BuildType
    } else {
        Write-Warn "Skipping SDK installation per -SkipInstall"
    }
} else {
    Write-Warn "Skipping SDK build per -SkipSdk"
}

$demoExe = $null
if (-not $SkipDemo) {
    $tigerHeaders = if ((-not $SkipInstall) -and (Test-Path (Join-Path $InstallPrefix "include"))) {
        Join-Path $InstallPrefix "include"
    } else {
        Join-Path $RepoRoot "include"
    }
    if (-not (Test-Path $tigerHeaders)) {
        throw "TigerAPI headers not found at $tigerHeaders"
    }

    $tigerLibrary = Resolve-TigerApiLibrary -InstallDir $InstallPrefix -BuildDir $BuildDir -Config $BuildType
    if (-not $tigerLibrary) {
        throw "Unable to locate tigerapi library. Build/install the SDK first."
    }

    $demoDefines = @{
        "CMAKE_MSVC_RUNTIME_LIBRARY" = Get-MsvcRuntimeLibrary -RuntimeFlavor $Runtime -Configuration $BuildType
        "TIGERAPI_INCLUDE_DIR"       = $tigerHeaders
        "TIGERAPI_LIBRARY"           = $tigerLibrary
        "BOOST_ROOT"                 = $boostLayout.Root
        "BOOST_INCLUDEDIR"           = $boostLayout.Include
        "BOOST_LIBRARYDIR"           = $boostLayout.Lib
        "Boost_USE_STATIC_LIBS"      = "ON"
        "Protobuf_ROOT"              = $protobufLayout.Root
        "Protobuf_DIR"               = Join-Path $protobufLayout.Root "lib/cmake/protobuf"
        "Protobuf_PROTOC_EXECUTABLE" = $protobufLayout.Protoc
        "Protobuf_USE_STATIC_LIBS"   = "OFF"
        "CPPREST_PREFIX"             = $tripletRoot
        "PROTOBUF_PREFIX"            = $protobufLayout.Root
        "CMAKE_PREFIX_PATH"          = ($boostLayout.Root, $protobufLayout.Root, $tripletRoot, $InstallPrefix -join ';')
        "VCPKG_TARGET_TRIPLET"       = $Triplet
    }

    $demoSource = Join-Path $RepoRoot "demo"
    Write-Info "Configuring demo"
    Invoke-CMakeConfigure -SourceDir $demoSource -BinaryDir $DemoBuildDir -BuildTypeValue $BuildType -GeneratorValue $Generator -Architecture $descriptor.ArchArg -ToolchainFile $toolchainFile -Definitions $demoDefines
    Write-Info "Building demo"
    Invoke-CMakeBuild -BinaryDir $DemoBuildDir -BuildTypeValue $BuildType -Targets @("openapi_cpp_test")
    $demoExe = Resolve-DemoExecutable -BuildDir $DemoBuildDir -Config $BuildType
    if (-not $demoExe) {
        Write-Warn "Demo executable not found after build."
    }
} else {
    Write-Warn "Skipping demo build per -SkipDemo"
}

if ($RunDemo -and $demoExe) {
    Write-Info "Running demo"
    $runtimePaths = @(
        (Split-Path $demoExe -Parent),
        (Join-Path $InstallPrefix "bin"),
        (Join-Path $InstallPrefix "lib"),
        (Join-Path $tripletRoot "bin"),
        (Join-Path $tripletRoot "debug/bin"),
        (Join-Path $tripletRoot "lib"),
        $boostLayout.Lib,
        $protobufLayout.Bin
    )
    Invoke-DemoExecutable -Executable $demoExe -RuntimePaths $runtimePaths
}

Write-Host ""
Write-Info "Windows build finished"
Write-Host "   Triplet          : $Triplet"
Write-Host "   Build type       : $BuildType"
Write-Host "   Runtime flavor   : $Runtime"
Write-Host "   Install prefix   : $InstallPrefix"
Write-Host "   vcpkg root       : $VcpkgRoot"
Write-Host "   Boost root       : $($boostLayout.Root)"
Write-Host "   Protobuf root    : $($protobufLayout.Root)"
if ($demoExe) {
    Write-Host "   Demo executable  : $demoExe"
    Write-Host "Configure tiger_openapi_config.properties before running the demo against live services."
} elseif ($RunDemo) {
    Write-Warn "Demo was requested but not found."
}

Write-Host ""
Write-Host "Recommended environment variables for consuming the SDK in other projects:"
Write-Host "    setx TIGERAPI_SDK_ROOT $InstallPrefix"
Write-Host "    setx BOOST_ROOT $($boostLayout.Root)"
Write-Host "    setx PROTOBUF_ROOT $($protobufLayout.Root)"
Write-Host "    setx VCPKG_ROOT $VcpkgRoot"
