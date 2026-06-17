<#
.SYNOPSIS
    Windows build orchestration script for the Tiger OpenAPI C++ SDK.

.DESCRIPTION
    This script codifies the Windows guidance from README.md:
            * Requires a Visual Studio C++ workload and CMake 3.15+.
            * Installs OpenSSL and cpprestsdk with vcpkg using the selected triplet.
            * Builds Boost 1.86.0 directly from its official source archive and caches it under .deps/.
            * Uses vcpkg Protobuf by default, while keeping a source-install Protobuf path for CMake parity
                with macOS/Linux.
            * Configures, builds, installs, and optionally runs the SDK demo using the prepared dependencies.

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

    [ValidateSet("Vcpkg","Source")]
    [string]$ProtobufProvider = "Vcpkg",

    [string]$BoostVersion = "1.86.0",
    [string]$ProtobufVersion = "5.28.3"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$ScriptRoot = if ($PSScriptRoot) { $PSScriptRoot } else { Split-Path -Parent $MyInvocation.MyCommand.Path }
$RepoRoot = (Resolve-Path (Join-Path $ScriptRoot ".." ) ).Path

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
$DepsSourceDir     = Join-Path $DepsRoot "src"
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
$null = Ensure-Directory -Path $DepsSourceDir
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

    $destinationDir = Split-Path -Parent $Destination
    if ($destinationDir) {
        Ensure-Directory -Path $destinationDir | Out-Null
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

    $tar = Get-Command tar -ErrorAction SilentlyContinue
    if ($tar) {
        try {
            Write-Info "Extracting $FriendlyName with tar"
            Ensure-Directory -Path $DestinationPath | Out-Null
            & $tar.Source -xf $ArchivePath -C $DestinationPath
            if ($LASTEXITCODE -eq 0) {
                return
            }
            Write-Warn "tar extraction of $FriendlyName exited with code $LASTEXITCODE; falling back to Expand-Archive."
        }
        catch {
            Write-Warn "tar extraction of $FriendlyName failed: $($_.Exception.Message); falling back to Expand-Archive."
            if (Test-Path $DestinationPath) {
                Remove-Item -Path $DestinationPath -Recurse -Force -ErrorAction SilentlyContinue
            }
        }
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
            $null = Download-Archive -Urls @($url) -Destination $archivePath -FriendlyName "CMake $PortableCmakeVersion"
        } elseif (-not (Test-ZipArchive -ArchivePath $archivePath)) {
            Remove-Item -Path $archivePath -Force -ErrorAction SilentlyContinue
            $null = Download-Archive -Urls @($url) -Destination $archivePath -FriendlyName "CMake $PortableCmakeVersion"
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
            $vcpkgArgs = @("install")
            if ($TripletValue) { $vcpkgArgs += "--triplet"; $vcpkgArgs += $TripletValue }
            & $VcpkgExe @vcpkgArgs
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
    $vcpkgArgs = @("install")
    foreach ($pkg in $Packages) {
        if ($TripletValue) {
            $vcpkgArgs += "${pkg}:$TripletValue"
        } else {
            $vcpkgArgs += $pkg
        }
    }
    $vcpkgArgs += "--clean-after-build"
    & $VcpkgExe @vcpkgArgs
    if ($LASTEXITCODE -ne 0) {
        throw "vcpkg install failed for packages $($Packages -join ', ')"
    }
}

function Resolve-VcpkgTripletRoot {
    param([string]$TripletValue)

    $candidates = @()
    if ($HasVcpkgManifest) {
        if ($TripletValue -match "x86") {
            $candidates += Join-Path (Join-Path $RepoRoot "vcpkg_installed_x86") $TripletValue
        }
        $candidates += Join-Path (Join-Path $RepoRoot "vcpkg_installed") $TripletValue
    }
    $candidates += Join-Path (Join-Path $VcpkgRoot "installed") $TripletValue

    foreach ($candidate in $candidates) {
        if (Test-Path $candidate) {
            return (Resolve-Path $candidate).Path
        }
    }

    throw "Unable to locate vcpkg installed triplet root for '$TripletValue'. Checked: $($candidates -join '; ')"
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

function Get-ProtobufReleaseVersion {
    param([string]$Version)

    if ($Version -match '^5\.(\d+\.\d+)$') {
        return $Matches[1]
    }
    return $Version
}

function Resolve-ProtobufCMakeDir {
    param([string]$Root)

    $candidates = @(
        (Join-Path $Root "share\protobuf"),
        (Join-Path $Root "lib\cmake\protobuf"),
        (Join-Path $Root "cmake")
    )

    foreach ($candidate in $candidates) {
        if (Test-Path (Join-Path $candidate "protobuf-config.cmake")) {
            return $candidate
        }
    }

    throw "Unable to locate protobuf-config.cmake under '$Root'."
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

    $sourceDir = Join-Path $DepsSourceDir "boost_${token}"
    $hasFiles = $false
    if (Test-Path $sourceDir) {
        $hasFiles = $null -ne (Get-ChildItem -Path $sourceDir -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1)
    }
    if (-not $hasFiles) {
        Write-Info "Extracting Boost $Version"
        Extract-Archive -ArchivePath $archivePath -DestinationPath $DepsSourceDir -RetryUrls $urls -FriendlyName "Boost $Version"
    }

    $tripletRoot = Ensure-Directory -Path (Join-Path $DepsInstallDir $Triplet)
    $installDir = Join-Path $tripletRoot "boost-$Version"
    $boostVariant = if ($BuildType -like "Debug*") { "debug" } else { "release" }
    $boostRuntimeLink = if ($Runtime -eq "MT") { "static" } else { "shared" }
    $stamp = Join-Path $installDir ".complete-$BuildType-$Runtime"
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
                "link=static",
                "runtime-link=$boostRuntimeLink",
                "threading=multi",
                "variant=$boostVariant",
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
        $libDir = if ($BuildType -like "Debug*") { Join-Path $resolved "debug\lib" } else { Join-Path $resolved "lib" }
        $releaseLibDir = Join-Path $resolved "lib"
        $binDir = Join-Path $resolved "bin"
        $toolsDir = Join-Path $resolved "tools\protobuf"
        $cmakeDir = Resolve-ProtobufCMakeDir -Root $resolved
        $protoc = Join-Path $toolsDir "protoc.exe"
        if (-not (Test-Path $protoc)) {
            $protoc = Join-Path $binDir "protoc.exe"
        }
        $protobufLibName = if ($BuildType -like "Debug*") { "libprotobufd.lib" } else { "libprotobuf.lib" }
        $protobufLib = Join-Path $libDir $protobufLibName
        foreach ($path in @($includeDir,$libDir,$releaseLibDir,$binDir,$cmakeDir,$protoc,$protobufLib)) {
            if (-not (Test-Path $path)) {
                throw "Provided Protobuf root '$resolved' is missing $path."
            }
        }
        return [pscustomobject]@{ Root=$resolved; Include=$includeDir; Lib=$libDir; ReleaseLib=$releaseLibDir; Bin=$binDir; CMakeDir=$cmakeDir; Protoc=$protoc; Library=$protobufLib }
    }

    if ($SkipDeps) {
        throw "Protobuf root not provided and -SkipDeps was supplied."
    }

    if ($ProtobufProvider -eq "Source") {
        $releaseVersion = Get-ProtobufReleaseVersion -Version $Version
        $archiveName = "protobuf-$releaseVersion.zip"
        $archivePath = Join-Path $DownloadsDir $archiveName
        $downloadUrl = "https://github.com/protocolbuffers/protobuf/releases/download/v$releaseVersion/$archiveName"

        if (-not (Test-Path $archivePath)) {
            $null = Download-Archive -Urls @($downloadUrl) -Destination $archivePath -FriendlyName "Protobuf $Version"
        } elseif (-not (Test-ZipArchive -ArchivePath $archivePath)) {
            Remove-Item -Path $archivePath -Force
            $null = Download-Archive -Urls @($downloadUrl) -Destination $archivePath -FriendlyName "Protobuf $Version"
        }

        $sourceDir = Join-Path $DepsSourceDir "protobuf-$releaseVersion"
        $hasFiles = $false
        if (Test-Path $sourceDir) {
            $hasFiles = $null -ne (Get-ChildItem -Path $sourceDir -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1)
        }
        if (-not $hasFiles) {
            Write-Info "Extracting Protobuf $Version source"
            Extract-Archive -ArchivePath $archivePath -DestinationPath $DepsSourceDir -RetryUrls @($downloadUrl) -FriendlyName "Protobuf $Version"
        }

        $tripletInstallRoot = Ensure-Directory -Path (Join-Path $DepsInstallDir $Triplet)
        $runtimeToken = $Runtime.ToUpperInvariant()
        $installDir = Join-Path $tripletInstallRoot "protobuf-$Version-$runtimeToken"
        $stamp = Join-Path $installDir ".complete-$BuildType"
        if (-not (Test-Path $stamp)) {
            if (Test-Path $installDir) {
                Remove-Item -Path $installDir -Recurse -Force
            }
            Ensure-Directory -Path $installDir | Out-Null

            $buildDir = Join-Path $DepsBuildDir "protobuf-$Triplet-$runtimeToken-$BuildType"
            if (Test-Path $buildDir) {
                Remove-Item -Path $buildDir -Recurse -Force
            }
            Ensure-Directory -Path $buildDir | Out-Null

            $runtimeLibrary = Get-MsvcRuntimeLibrary -RuntimeFlavor $Runtime -Configuration $BuildType
            $staticRuntime = if ($Runtime.ToUpperInvariant() -eq "MT") { "ON" } else { "OFF" }

            $protobufCmakeArgs = @("-S", $sourceDir, "-B", $buildDir,
                "-DCMAKE_INSTALL_PREFIX=$installDir",
                "-DCMAKE_TOOLCHAIN_FILE=$(Join-Path $VcpkgRoot 'scripts\buildsystems\vcpkg.cmake')",
                "-DVCPKG_TARGET_TRIPLET=$Triplet",
                "-Dprotobuf_BUILD_TESTS=OFF",
                "-Dprotobuf_BUILD_CONFORMANCE=OFF",
                "-Dprotobuf_BUILD_EXAMPLES=OFF",
                "-Dprotobuf_BUILD_SHARED_LIBS=ON",
                "-Dprotobuf_ABSL_PROVIDER=package",
                "-Dprotobuf_UTF8_RANGE_PROVIDER=package",
                "-Dprotobuf_MSVC_STATIC_RUNTIME=$staticRuntime",
                "-DCMAKE_MSVC_RUNTIME_LIBRARY=$runtimeLibrary"
            )
            if ($Generator) { $protobufCmakeArgs = @("-G", $Generator) + $protobufCmakeArgs }
            if ($Descriptor -and $Descriptor.ArchArg -and ($Generator -match "Visual Studio")) {
                $protobufCmakeArgs = @("-A", $Descriptor.ArchArg) + $protobufCmakeArgs
            }

            Write-Info "Configuring Protobuf $Version from source"
            cmake @protobufCmakeArgs
            if ($LASTEXITCODE -ne 0) { throw "Protobuf source configure failed." }

            Write-Info "Building Protobuf $Version from source"
            cmake --build $buildDir --config $BuildType --target install --parallel $ProcessorCount
            if ($LASTEXITCODE -ne 0) { throw "Protobuf source build failed." }

            New-Item -ItemType File -Path $stamp -Force | Out-Null
        }

        $root = (Resolve-Path $installDir).Path
        $includeDir = Join-Path $root "include"
        $configLibDir = Join-Path $root "lib"
        $releaseLibDir = Join-Path $root "lib"
        $binDir = Join-Path $root "bin"
        $cmakeDir = Resolve-ProtobufCMakeDir -Root $root
        $protoc = Join-Path $binDir "protoc.exe"
        $protobufLibName = if ($BuildType -like "Debug*") { "libprotobufd.lib" } else { "libprotobuf.lib" }
        $protobufLib = Join-Path $configLibDir $protobufLibName

        foreach ($path in @($includeDir,$configLibDir,$releaseLibDir,$binDir,$cmakeDir,$protoc,$protobufLib)) {
            if (-not (Test-Path $path)) {
                throw "Missing source-built Protobuf component '$path'."
            }
        }

        return [pscustomobject]@{ Root=$root; Include=$includeDir; Lib=$configLibDir; ReleaseLib=$releaseLibDir; Bin=$binDir; CMakeDir=$cmakeDir; Protoc=$protoc; Library=$protobufLib }
    }

    $root = Resolve-VcpkgTripletRoot -TripletValue $Triplet
    $includeDir = Join-Path $root "include"
    $configLibDir = if ($BuildType -like "Debug*") { Join-Path $root "debug\lib" } else { Join-Path $root "lib" }
    $releaseLibDir = Join-Path $root "lib"
    $binDir = Join-Path $root "bin"
    $toolsDir = Join-Path $root "tools\protobuf"
    $cmakeDir = Join-Path $root "share\protobuf"
    $protoc = Join-Path $toolsDir "protoc.exe"
    if (-not (Test-Path $protoc)) {
        $protoc = Join-Path $binDir "protoc.exe"
    }

    $protobufLibName = if ($BuildType -like "Debug*") { "libprotobufd.lib" } else { "libprotobuf.lib" }
    $protobufLib = Join-Path $configLibDir $protobufLibName

    foreach ($path in @($includeDir,$configLibDir,$releaseLibDir,$binDir,$cmakeDir,$protoc,$protobufLib)) {
        if (-not (Test-Path $path)) {
            throw "Missing vcpkg Protobuf component '$path'. Run vcpkg install for triplet '$Triplet'."
        }
    }

    return [pscustomobject]@{ Root=$root; Include=$includeDir; Lib=$configLibDir; ReleaseLib=$releaseLibDir; Bin=$binDir; CMakeDir=$cmakeDir; Protoc=$protoc; Library=$protobufLib }
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
    $cmakeArgs = @("-S", $SourceDir, "-B", $BinaryDir, "-DCMAKE_BUILD_TYPE=$BuildTypeValue")
    if ($GeneratorValue) { $cmakeArgs = @("-G", $GeneratorValue) + $cmakeArgs }
    if ($Architecture) { $cmakeArgs = @("-A", $Architecture) + $cmakeArgs }
    if ($ToolchainFile) { $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile" }
    foreach ($key in $Definitions.Keys) {
        $value = $Definitions[$key]
        if ($null -ne $value -and $value -ne "") {
            $cmakeArgs += "-D${key}=$value"
        }
    }
    cmake @cmakeArgs
}

function Invoke-CMakeBuild {
    param([string]$BinaryDir,[string]$BuildTypeValue,[string[]]$Targets)
    $cmakeBuildArgs = @("--build", $BinaryDir, "--config", $BuildTypeValue, "--parallel", $ProcessorCount)
    if ($Targets -and $Targets.Count -gt 0) { $cmakeBuildArgs += "--target"; $cmakeBuildArgs += $Targets }
    cmake @cmakeBuildArgs
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

Write-Info "Preparing Boost source dependency and Protobuf ($ProtobufProvider provider)"
$boostLayout = Prepare-BoostDependency -Version $BoostVersion -Descriptor $descriptor
$protobufLayout = Prepare-ProtobufDependency -Version $ProtobufVersion -Descriptor $descriptor
$tripletRoot = Resolve-VcpkgTripletRoot -TripletValue $Triplet

if (-not $SkipSdk) {
    Write-Info "Configuring Tiger OpenAPI SDK"
    $sdkDefines = @{
        "CMAKE_INSTALL_PREFIX"       = $InstallPrefix
        "CMAKE_MSVC_RUNTIME_LIBRARY" = Get-MsvcRuntimeLibrary -RuntimeFlavor $Runtime -Configuration $BuildType
        "Boost_ROOT"                 = $boostLayout.Root
        "BOOST_INCLUDEDIR"           = $boostLayout.Include
        "BOOST_LIBRARYDIR"           = $boostLayout.Lib
        "Boost_USE_STATIC_LIBS"      = "ON"
        "Protobuf_ROOT"              = $protobufLayout.Root
        "Protobuf_DIR"               = $protobufLayout.CMakeDir
        "Protobuf_PROTOC_EXECUTABLE" = $protobufLayout.Protoc
        "Protobuf_INCLUDE_DIR"       = $protobufLayout.Include
        "Protobuf_LIBRARY"           = $protobufLayout.Library
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
        "Boost_ROOT"                 = $boostLayout.Root
        "BOOST_INCLUDEDIR"           = $boostLayout.Include
        "BOOST_LIBRARYDIR"           = $boostLayout.Lib
        "Boost_USE_STATIC_LIBS"      = "ON"
        "Protobuf_ROOT"              = $protobufLayout.Root
        "Protobuf_DIR"               = $protobufLayout.CMakeDir
        "Protobuf_PROTOC_EXECUTABLE" = $protobufLayout.Protoc
        "Protobuf_INCLUDE_DIR"       = $protobufLayout.Include
        "Protobuf_LIBRARY"           = $protobufLayout.Library
        "Protobuf_USE_STATIC_LIBS"   = "OFF"
        "CPPREST_PREFIX"             = $tripletRoot
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
