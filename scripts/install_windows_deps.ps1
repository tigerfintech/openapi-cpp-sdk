<#
.SYNOPSIS
    Installs the toolchain, builds the Tiger OpenAPI C++ SDK, and produces multi-runtime Windows binaries (plus demo).

.DESCRIPTION
    This script bootstraps vcpkg (or reuses an existing checkout) and installs the
    libraries used by the SDK (Boost, OpenSSL, cpprestsdk, protobuf). It can also
    ensure that core command-line tools such as Git and CMake are present by
    leveraging winget when the -InstallPrereqs switch is supplied. After
    dependencies are ready, the script configures and builds the SDK plus demo via
    CMake. By default it produces eight Windows library variants:

        output/Windows/x64/{Release,Debug}-{MD,MT}
        output/Windows/Win32/{Release,Debug}-{MD,MT}

    Use -SingleBuild to fall back to the legacy "one configuration" workflow.

.PARAMETER Triplet
    vcpkg triplet to install (default: x64-windows). Other common values include
    x86-windows, arm64-windows, and their "-static" counterparts.

.PARAMETER VcpkgRoot
    Where vcpkg should live. Defaults to <repo>/.vcpkg next to this script.

.PARAMETER UpdateVcpkg
    Pulls the latest vcpkg commits before installing packages.

.PARAMETER ForceBootstrap
    Forces re-running bootstrap-vcpkg even when vcpkg.exe already exists.

.PARAMETER InstallPrereqs
    Attempts to install Git and CMake via winget when they are missing.
    Requires an elevated PowerShell session.

.PARAMETER SkipPackageInstall
    Stops after bootstrapping vcpkg (useful when you only want the toolchain).

.PARAMETER BuildType
    CMake configuration to build (default: Release).

.PARAMETER Generator
    Optional CMake generator name (e.g., "Visual Studio 17 2022"). If omitted,
    CMake picks a default based on the environment.

.PARAMETER SdkBuildDir
    Build directory root for the SDK (default: <repo>/build/windows). Each
    configuration/architecture/runtime combination gets its own subfolder.

.PARAMETER DemoBuildDir
    Build directory root for the demo (default: <repo>/demo/build/windows).

.PARAMETER InstallPrefix
    Base destination for `cmake --install`. Defaults to <repo>/output/Windows.
    Matrix builds create `<InstallPrefix>/<arch>/<config>-<runtime>` subfolders;
    single-build mode uses the value as-is (defaulting to output/Windows/tigerapi-sdk).

.PARAMETER SkipSdkBuild
    Skip configuring/building the SDK (still installs dependencies).

.PARAMETER SkipSdkInstall
    Build the SDK but skip `cmake --install`.

.PARAMETER SkipDemoBuild
    Skip configuring/building the demo.

.PARAMETER RunDemo
    Execute the demo binary after a successful build (requires config files).

.PARAMETER RuntimeFlavors
    (Single-build mode only) Which MSVC runtime flavor(s) to use: MT or MD. Default MD.

.PARAMETER SingleBuild
    Disable the matrix build and only build the triplet specified via -Triplet and
    the configuration specified via -BuildType.

.EXAMPLE
    # Default usage (x64 build, reuse existing vcpkg if present)
    powershell -ExecutionPolicy Bypass -File scripts/install_windows_deps.ps1

.EXAMPLE
    # Install prerequisites via winget, update vcpkg, and target ARM64
    powershell -ExecutionPolicy Bypass -File scripts/install_windows_deps.ps1 `
      -InstallPrereqs -UpdateVcpkg -Triplet arm64-windows

.NOTES
    - Visual Studio 2019/2022 with "Desktop development with C++" is still required.
    - winget installs need Administrator privileges.
#>
[CmdletBinding()]
param(
    [ValidateSet("x64-windows","x86-windows","arm64-windows","x64-windows-static","x86-windows-static","arm64-windows-static")]
    [string]$Triplet = "x64-windows",

    [string]$VcpkgRoot = (Join-Path $PSScriptRoot ".." | Resolve-Path | ForEach-Object { Join-Path $_ ".vcpkg" }),

    [switch]$UpdateVcpkg,
    [switch]$ForceBootstrap,
    [switch]$InstallPrereqs,
    [switch]$SkipPackageInstall,

    [string]$BuildType = "Release",
    [string]$Generator,
    [string]$SdkBuildDir,
    [string]$DemoBuildDir,
    [string]$InstallPrefix,
    [switch]$SkipSdkBuild,
    [switch]$SkipSdkInstall,
    [switch]$SkipDemoBuild,
    [switch]$RunDemo,

    [ValidateSet("MD","MT")]
    [string[]]$RuntimeFlavors = @("MD"),

    [switch]$SingleBuild
)

$ErrorActionPreference = "Stop"

$RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot ".." ) ).Path
if (-not $SdkBuildDir) {
    $SdkBuildDir = Join-Path $RepoRoot "build/windows"
}
if (-not $DemoBuildDir) {
    $DemoBuildDir = Join-Path $RepoRoot "demo/build/windows"
}
$defaultInstallBase = Join-Path $RepoRoot "output/Windows"
if (-not $PSBoundParameters.ContainsKey('InstallPrefix')) {
    if ($SingleBuild) {
        $InstallPrefix = Join-Path $defaultInstallBase "tigerapi-sdk"
    } else {
        $InstallPrefix = $defaultInstallBase
    }
}
$InstallPrefix = $InstallPrefix.TrimEnd('\/')
if ([string]::IsNullOrWhiteSpace($InstallPrefix)) {
    throw "InstallPrefix cannot be empty."
}
$SdkBuildRoot = $SdkBuildDir
$DemoBuildRoot = $DemoBuildDir

function Write-Info {
    param([string]$Message)
    Write-Host "[+] $Message" -ForegroundColor Green
}

function Write-Warn {
    param([string]$Message)
    Write-Host "[!] $Message" -ForegroundColor Yellow
}

function Require-Command {
    param(
        [string]$Command,
        [string]$FriendlyName,
        [string]$WingetId
    )

    if (Get-Command $Command -ErrorAction SilentlyContinue) {
        return
    }

    if (-not $InstallPrereqs) {
        throw "Required tool '$FriendlyName' (command '$Command') is missing. Re-run with -InstallPrereqs or install it manually."
    }

    if (-not (Get-Command winget.exe -ErrorAction SilentlyContinue)) {
        throw "winget is not available, cannot install '$FriendlyName' automatically. Install it manually or disable -InstallPrereqs."
    }

    if (-not ([Security.Principal.WindowsPrincipal][Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
        throw "Installing '$FriendlyName' requires an elevated PowerShell window when using -InstallPrereqs."
    }

    Write-Info "Installing $FriendlyName via winget ($WingetId)..."
    winget install --id $WingetId --exact --silent --accept-source-agreements --accept-package-agreements | Out-Null
    if (-not (Get-Command $Command -ErrorAction SilentlyContinue)) {
        throw "Failed to install '$FriendlyName' (command '$Command'). Check winget logs for details."
    }
}

function Ensure-VcpkgRepository {
    if (-not (Test-Path $VcpkgRoot)) {
        Write-Info "Cloning vcpkg into $VcpkgRoot"
        git clone https://github.com/microsoft/vcpkg $VcpkgRoot | Out-Null
        return
    }

    if ($UpdateVcpkg) {
        Write-Info "Updating existing vcpkg checkout"
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
        Write-Info "Bootstrapping vcpkg (this may take a minute)"
        $bootstrap = Join-Path $VcpkgRoot "bootstrap-vcpkg.bat"
        & $bootstrap -disableMetrics | Out-Null
    }
    if (-not (Test-Path $vcpkgExe)) {
        throw "vcpkg.exe not found after bootstrap."
    }
    return $vcpkgExe
}

function Install-Packages {
    param(
        [string]$VcpkgExe,
        [string]$TripletValue,
        [string[]]$Packages
    )

    if ($SkipPackageInstall) {
        Write-Warn "Skipping dependency installation because -SkipPackageInstall was set."
        return
    }

    Write-Info "Installing SDK dependencies for triplet '$TripletValue'"
    $packageArgs = @()
    foreach ($pkg in $Packages) {
        $packageArgs += "${pkg}:$TripletValue"
    }
    & $VcpkgExe install @packageArgs --clean-after-build
}

function Get-TripletArchitecture {
    param([string]$TripletValue)

    if ($TripletValue -match "x64") {
        return "x64"
    }
    if ($TripletValue -match "x86") {
        return "Win32"
    }
    if ($TripletValue -match "arm64") {
        return "ARM64"
    }
    return $null
}

function Invoke-CMakeConfigure {
    param(
        [string]$SourceDir,
        [string]$BuildDir,
        [string]$BuildType,
        [string]$Generator,
        [string]$Architecture,
        [string]$ToolchainFile,
        [string]$TripletValue,
        [hashtable]$Definitions
    )

    if (-not $Definitions) {
        $Definitions = @{}
    }

    New-Item -ItemType Directory -Path $BuildDir -Force | Out-Null
    $args = @("-S", $SourceDir, "-B", $BuildDir, "-DCMAKE_BUILD_TYPE=$BuildType")
    if ($Generator) {
        $args = @("-G", $Generator) + $args
    }
    if ($Architecture) {
        $args = @("-A", $Architecture) + $args
    }
    if ($ToolchainFile) {
        $args += "-DCMAKE_TOOLCHAIN_FILE=$ToolchainFile"
    }
    if ($TripletValue) {
        $args += "-DVCPKG_TARGET_TRIPLET=$TripletValue"
    }
    foreach ($key in $Definitions.Keys) {
        $value = $Definitions[$key]
        if ($null -ne $value -and $value -ne "") {
            $args += "-D${key}=$value"
        }
    }
    cmake @args
}

function Invoke-CMakeBuild {
    param(
        [string]$BuildDir,
        [string]$BuildType,
        [string[]]$Targets
    )

    $args = @("--build", $BuildDir, "--config", $BuildType)
    if ($Targets -and $Targets.Count -gt 0) {
        $args += "--target"
        $args += $Targets
    }
    cmake @args
}

function Invoke-CMakeInstall {
    param(
        [string]$BuildDir,
        [string]$BuildType
    )

    cmake --install $BuildDir --config $BuildType
}

function Resolve-TigerApiLibrary {
    param(
        [string]$InstallDir,
        [string]$SdkBuildDir,
        [string]$BuildType
    )

    $searchDirs = @(
        (Join-Path $InstallDir "lib"),
        (Join-Path $InstallDir "bin"),
        $SdkBuildDir,
        (Join-Path $SdkBuildDir $BuildType)
    )
    foreach ($dir in $searchDirs) {
        if (-not (Test-Path $dir)) { continue }
        foreach ($pattern in @("tigerapi*.lib", "tigerapi*.dll", "tigerapi*.a")) {
            $candidate = Get-ChildItem -Path $dir -Filter $pattern -File -ErrorAction SilentlyContinue | Select-Object -First 1
            if ($candidate) {
                return $candidate.FullName
            }
        }
    }
    return $null
}

function Resolve-DemoExecutable {
    param(
        [string]$DemoBuildDir,
        [string]$BuildType
    )

    $candidates = @(
        (Join-Path $DemoBuildDir "openapi_cpp_test.exe"),
        (Join-Path $DemoBuildDir (Join-Path $BuildType "openapi_cpp_test.exe"))
    )
    foreach ($path in $candidates) {
        if (Test-Path $path) {
            return $path
        }
    }
    return $null
}

function Get-MsvcRuntimeLibrary {
    param(
        [string]$RuntimeFlavor,
        [string]$Configuration
    )

    $isDebug = $Configuration -like "*Debug*"
    switch ($RuntimeFlavor) {
        "MT" { return $isDebug ? "MultiThreadedDebug" : "MultiThreaded" }
        default { return $isDebug ? "MultiThreadedDebugDLL" : "MultiThreadedDLL" }
    }
}

Write-Info "Validating host prerequisites"
Require-Command -Command git -FriendlyName "Git" -WingetId "Git.Git"
Require-Command -Command cmake -FriendlyName "CMake" -WingetId "Kitware.CMake"

Ensure-VcpkgRepository
$vcpkgExe = Bootstrap-Vcpkg
if ($SingleBuild) {
    if (-not $Triplet) {
        throw "Triplet must be specified when -SingleBuild is used."
    }
    $buildEntries = @()
    $archArg = Get-TripletArchitecture -TripletValue $Triplet
    if (-not $archArg) {
        throw "Unable to derive CMake architecture from triplet '$Triplet'."
    }
    $archLabel = if ($archArg -eq "Win32") { "Win32" } elseif ($archArg -eq "ARM64") { "ARM64" } else { "x64" }
    $buildEntries += [pscustomobject]@{
        Triplet = $Triplet
        ArchArg = $archArg
        ArchLabel = $archLabel
    }
    $buildTypes = @($BuildType)
    $runtimeSet = $RuntimeFlavors
} else {
    $buildEntries = @(
        [pscustomobject]@{ Triplet = "x64-windows"; ArchArg = "x64";  ArchLabel = "x64" },
        [pscustomobject]@{ Triplet = "x86-windows"; ArchArg = "Win32"; ArchLabel = "Win32" }
    )
    $buildTypes = @("Release", "Debug")
    $runtimeSet = @("MT", "MD")
    if ($PSBoundParameters.ContainsKey('RuntimeFlavors')) {
        Write-Warn "RuntimeFlavors applies only when -SingleBuild is specified; ignoring provided value in matrix mode."
    }
}

$uniqueTriplets = $buildEntries.Triplet | Sort-Object -Unique
foreach ($tripletValue in $uniqueTriplets) {
    Install-Packages -VcpkgExe $vcpkgExe -TripletValue $tripletValue -Packages @("boost", "openssl", "cpprestsdk", "protobuf")
}

$env:VCPKG_ROOT = $VcpkgRoot
$env:VCPKG_DEFAULT_TRIPLET = $uniqueTriplets[0]
$toolchainFile = Join-Path $VcpkgRoot "scripts/buildsystems/vcpkg.cmake"
if (-not (Test-Path $toolchainFile)) {
    throw "CMake toolchain file not found at $toolchainFile. Ensure vcpkg bootstrap succeeded."
}

$buildResults = @()
$demoExecutables = @()

foreach ($entry in $buildEntries) {
    $tripletRoot = Join-Path $VcpkgRoot "installed/$($entry.Triplet)"
    foreach ($config in $buildTypes) {
        foreach ($runtime in $runtimeSet) {
            $comboLabel = "$($entry.ArchLabel)/$config-$runtime"
            $runtimeLib = Get-MsvcRuntimeLibrary -RuntimeFlavor $runtime -Configuration $config
            $sdkBuildDirCurrent = if ($SingleBuild) { $SdkBuildRoot } else { Join-Path $SdkBuildRoot "$comboLabel" }
            $demoBuildDirCurrent = if ($SingleBuild) { $DemoBuildRoot } else { Join-Path $DemoBuildRoot "$comboLabel" }
            $installDirCurrent = if ($SingleBuild) { $InstallPrefix } else { Join-Path $InstallPrefix "$comboLabel" }
            New-Item -ItemType Directory -Force -Path $installDirCurrent | Out-Null

            $sdkBuilt = $false
            $sdkInstalled = $false
            $demoBuilt = $false
            $demoExePath = $null

            if ($SkipSdkBuild) {
                Write-Warn "[$comboLabel] Skipping SDK build because -SkipSdkBuild is set."
            } else {
                Write-Info "[$comboLabel] Configuring Tiger OpenAPI SDK"
                $sdkDefines = @{
                    "CMAKE_INSTALL_PREFIX" = $installDirCurrent
                    "CMAKE_MSVC_RUNTIME_LIBRARY" = $runtimeLib
                }
                if (Test-Path $tripletRoot) {
                    $sdkDefines["CMAKE_PREFIX_PATH"] = $tripletRoot
                }
                Invoke-CMakeConfigure -SourceDir $RepoRoot -BuildDir $sdkBuildDirCurrent -BuildType $config -Generator $Generator -Architecture $entry.ArchArg -ToolchainFile $toolchainFile -TripletValue $entry.Triplet -Definitions $sdkDefines
                Write-Info "[$comboLabel] Building Tiger OpenAPI SDK"
                Invoke-CMakeBuild -BuildDir $sdkBuildDirCurrent -BuildType $config
                $sdkBuilt = $true
                if ($SkipSdkInstall) {
                    Write-Warn "[$comboLabel] Skipping SDK installation because -SkipSdkInstall is set."
                } else {
                    Write-Info "[$comboLabel] Installing Tiger OpenAPI SDK -> $installDirCurrent"
                    Invoke-CMakeInstall -BuildDir $sdkBuildDirCurrent -BuildType $config
                    $sdkInstalled = $true
                }
            }

            if ($SkipDemoBuild) {
                Write-Warn "[$comboLabel] Skipping demo build because -SkipDemoBuild is set."
            } else {
                $tigerapiIncludeDir = if ($sdkInstalled) { Join-Path $installDirCurrent "include" } else { Join-Path $RepoRoot "include" }
                if (-not (Test-Path $tigerapiIncludeDir)) {
                    throw "[$comboLabel] TigerAPI headers not found at $tigerapiIncludeDir. Build/install the SDK first or supply -SkipDemoBuild."
                }
                $tigerapiLibrary = Resolve-TigerApiLibrary -InstallDir $installDirCurrent -SdkBuildDir $sdkBuildDirCurrent -BuildType $config
                if (-not $tigerapiLibrary) {
                    throw "[$comboLabel] Unable to locate tigerapi library. Ensure the SDK build succeeded or rerun with -SkipDemoBuild."
                }
                $demoDefines = @{
                    "TIGERAPI_INCLUDE_DIR" = $tigerapiIncludeDir
                    "TIGERAPI_LIBRARY" = $tigerapiLibrary
                    "CMAKE_MSVC_RUNTIME_LIBRARY" = $runtimeLib
                }
                $prefixValues = @()
                if ($sdkInstalled -and (Test-Path $installDirCurrent)) {
                    $prefixValues += $installDirCurrent
                }
                if (Test-Path $tripletRoot) {
                    $prefixValues += $tripletRoot
                    $demoDefines["CPPREST_PREFIX"] = $tripletRoot
                    $demoDefines["PROTOBUF_PREFIX"] = $tripletRoot
                    $demoDefines["BOOST_ROOT"] = $tripletRoot
                }
                if ($prefixValues.Count -gt 0) {
                    $demoDefines["CMAKE_PREFIX_PATH"] = ($prefixValues -join ';')
                }
                $demoSource = Join-Path $RepoRoot "demo"
                Write-Info "[$comboLabel] Configuring demo project"
                Invoke-CMakeConfigure -SourceDir $demoSource -BuildDir $demoBuildDirCurrent -BuildType $config -Generator $Generator -Architecture $entry.ArchArg -ToolchainFile $toolchainFile -TripletValue $entry.Triplet -Definitions $demoDefines
                Write-Info "[$comboLabel] Building demo executable"
                Invoke-CMakeBuild -BuildDir $demoBuildDirCurrent -BuildType $config -Targets @("openapi_cpp_test")
                $demoBuilt = $true
                $demoExePath = Resolve-DemoExecutable -DemoBuildDir $demoBuildDirCurrent -BuildType $config
                if ($RunDemo) {
                    if ($demoExePath -and (Test-Path $demoExePath)) {
                        Write-Info "[$comboLabel] Running demo (ensure tiger_openapi_config.properties exists)."
                        $demoWorkingDir = Split-Path $demoExePath -Parent
                        Push-Location $demoWorkingDir
                        try {
                            & $demoExePath
                        }
                        catch {
                            Write-Warn "[$comboLabel] Demo execution failed: $($_.Exception.Message)"
                        }
                        finally {
                            Pop-Location
                        }
                    } else {
                        Write-Warn "[$comboLabel] Demo executable not found; cannot run demo."
                    }
                }
                if ($demoExePath) {
                    $demoExecutables += $demoExePath
                }
            }

            $buildResults += [pscustomobject]@{
                Arch      = $entry.ArchLabel
                Triplet   = $entry.Triplet
                Config    = $config
                Runtime   = $runtime
                InstallDir = $installDirCurrent
                SdkBuilt  = $sdkBuilt
                SdkInstalled = $sdkInstalled
                DemoBuilt = $demoBuilt
                DemoExe   = $demoExePath
            }
        }
    }
}

Write-Host ""
Write-Info "Windows dependency + build pipeline finished."
Write-Host "   vcpkg root        : $VcpkgRoot"
Write-Host "   Toolchain file    : $toolchainFile"
Write-Host "   Generator         : $(if ($Generator) { $Generator } else { '(cmake default)' })"
Write-Host "   Build root        : $SdkBuildRoot"
Write-Host "   Install root      : $InstallPrefix"
Write-Host "   Demo build root   : $DemoBuildRoot"

foreach ($result in $buildResults) {
    $status = if ($result.SdkInstalled) { "SDK installed" } elseif ($result.SdkBuilt) { "SDK built" } else { "SDK skipped" }
    $demoStatus = if ($result.DemoBuilt) { "demo built" } elseif ($SkipDemoBuild) { "demo skipped" } else { "demo not built" }
    Write-Host ("   - {0} {1}-{2}: {3}, {4}, output -> {5}" -f $result.Arch, $result.Config, $result.Runtime, $status, $demoStatus, $result.InstallDir)
}

if (-not $RunDemo -and $demoExecutables.Count -gt 0) {
    Write-Host ""
    Write-Host "Run a demo manually (after configuring credentials), e.g.:"
    Write-Host "    `"$($demoExecutables[0])`""
}

Write-Host ""
Write-Host "Persist vcpkg variables for future shells if needed:"
Write-Host "    setx VCPKG_ROOT $VcpkgRoot"
Write-Host "    setx VCPKG_DEFAULT_TRIPLET $($uniqueTriplets[0])"
