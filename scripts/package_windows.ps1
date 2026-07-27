<#
Windows SDK packaging script.

Builds the Visual Studio SDK project for Win32/x64 across Debug-MD,
Debug-MT, Release-MD, and Release-MT, then recreates the matching zip files
under output/Windows/{Win32,x64}.

Examples:
  powershell -ExecutionPolicy Bypass -File .\scripts\package_windows.ps1
  powershell -ExecutionPolicy Bypass -File .\scripts\package_windows.ps1 -Platform x64 -Configuration Release-MD
  powershell -ExecutionPolicy Bypass -File .\scripts\package_windows.ps1 -SkipBuild
#>

[CmdletBinding()]
param(
    [ValidateSet('Win32', 'x64')]
    [string[]]$Platform = @('Win32', 'x64'),

    [ValidateSet('Debug-MD', 'Debug-MT', 'Release-MD', 'Release-MT')]
    [string[]]$Configuration = @('Debug-MD', 'Debug-MT', 'Release-MD', 'Release-MT'),

    [string]$RepoRoot,

    [string]$MSBuildPath = 'C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe',

    [switch]$SkipBuild,

    [switch]$SkipBoostBuild,

    [int]$BoostJobs = 0
)

$ErrorActionPreference = 'Stop'

if ([string]::IsNullOrWhiteSpace($RepoRoot)) {
    $RepoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
}

function Write-Step {
    param([string]$Message)
    Write-Host ''
    Write-Host "==> $Message"
}

function Get-BoostTriplet {
    param([string]$BuildPlatform)
    if ($BuildPlatform -eq 'Win32') { return 'x86-windows' }
    return 'x64-windows'
}

function Initialize-BoostVariant {
    param(
        [string]$BuildPlatform,
        [string]$BuildConfiguration,
        [string]$BoostSourceRoot,
        [string]$BoostInstallRoot
    )

    $stamp = Join-Path $BoostInstallRoot ".complete-$BuildConfiguration"
    if (Test-Path $stamp) { return }

    if ($SkipBoostBuild) {
        throw "Boost $BuildPlatform $BuildConfiguration is missing at $BoostInstallRoot. Remove -SkipBoostBuild to build it."
    }

    $variant = if ($BuildConfiguration.StartsWith('Debug')) { 'debug' } else { 'release' }
    $runtimeLink = if ($BuildConfiguration.EndsWith('-MT')) { 'static' } else { 'shared' }
    $addressModel = if ($BuildPlatform -eq 'Win32') { 32 } else { 64 }
    if ($BoostJobs -lt 0) { throw '-BoostJobs must be 0 or greater.' }
    if ($BoostJobs -eq 0) {
        $BoostJobs = if ($env:NUMBER_OF_PROCESSORS) { [int]$env:NUMBER_OF_PROCESSORS } else { 4 }
    }

    $boostLibRoot = Join-Path $BoostInstallRoot 'lib'
    New-Item -ItemType Directory -Force -Path $boostLibRoot | Out-Null

    Push-Location $BoostSourceRoot
    try {
        $b2 = Join-Path $BoostSourceRoot 'b2.exe'
        if (-not (Test-Path $b2)) {
            Write-Host "    Bootstrapping Boost build system..."
            & (Join-Path $BoostSourceRoot 'bootstrap.bat')
            if ($LASTEXITCODE -ne 0) { throw 'Boost bootstrap failed.' }
        }

        $libraries = @('chrono', 'filesystem', 'log', 'program_options', 'system', 'thread')
        $libraryArgs = $libraries | ForEach-Object { "--with-$_" }
        $b2Args = @(
            'toolset=msvc',
            "address-model=$addressModel",
            'architecture=x86',
            'link=static',
            "runtime-link=$runtimeLink",
            'threading=multi',
            "variant=$variant",
            '--layout=tagged',
            "--stagedir=$BoostInstallRoot",
            "-j$BoostJobs"
        ) + $libraryArgs + 'stage'

        Write-Host "    Building Boost 1.86.0 for $BuildPlatform $BuildConfiguration..."
        & $b2 @b2Args
        if ($LASTEXITCODE -ne 0) { throw "Boost build failed for $BuildPlatform $BuildConfiguration." }

        New-Item -ItemType File -Force -Path $stamp | Out-Null
    }
    finally {
        Pop-Location
    }
}

function Invoke-SdkBuild {
    param(
        [string]$BuildPlatform,
        [string]$BuildConfiguration,
        [string]$ProjectPath,
        [string]$BoostSourceRoot,
        [string]$BoostInstallRoot
    )

    $arguments = @(
        $ProjectPath,
        '/t:Rebuild',
        "/p:Configuration=$BuildConfiguration",
        "/p:Platform=$BuildPlatform",
        "/p:BOOST_ROOT=$BoostSourceRoot",
        "/p:BOOST_INSTALL_ROOT=$BoostInstallRoot",
        "/p:OPENAPI_BOOST_ROOT=$BoostSourceRoot",
        "/p:OPENAPI_BOOST_INSTALL_ROOT=$BoostInstallRoot",
        '/m',
        '/nr:false',
        '/v:minimal'
    )

    & $MSBuildPath @arguments
    if ($LASTEXITCODE -ne 0) {
        throw "MSBuild failed for $BuildPlatform $BuildConfiguration."
    }
}

function New-ZipPackage {
    param(
        [string]$BuildPlatform,
        [string]$BuildConfiguration,
        [string]$OutputRoot,
        [string]$RepositoryRoot
    )

    $packageDir = Join-Path $OutputRoot "$BuildPlatform\$BuildConfiguration"
    $zipPath = Join-Path $OutputRoot "$BuildPlatform\$BuildConfiguration.zip"

    if (-not (Test-Path $packageDir)) {
        throw "Output directory not found: $packageDir"
    }

    $installedRoot = if ($BuildPlatform -eq 'Win32') {
        Join-Path $RepositoryRoot 'vcpkg_installed_x86'
    }
    else {
        Join-Path $RepositoryRoot 'vcpkg_installed'
    }
    $triplet = Get-BoostTriplet -BuildPlatform $BuildPlatform
    $configurationDir = if ($BuildConfiguration.StartsWith('Debug')) { 'debug' } else { '' }
    $vcpkgRoot = Join-Path $installedRoot $triplet
    $vcpkgConfigRoot = if ($configurationDir) { Join-Path $vcpkgRoot $configurationDir } else { $vcpkgRoot }
    $vcpkgLibDir = Join-Path $vcpkgConfigRoot 'lib'
    $vcpkgBinDir = Join-Path $vcpkgConfigRoot 'bin'
    $debugSuffix = if ($BuildConfiguration.StartsWith('Debug')) { 'd' } else { '' }
    $stagingDir = Join-Path ([System.IO.Path]::GetTempPath()) ("tiger-openapi-windows-" + [guid]::NewGuid())

    function Copy-PackageFile {
        param([string]$Path)
        if (-not (Test-Path $Path -PathType Leaf)) {
            throw "Required package file not found: $Path"
        }
        Copy-Item $Path -Destination $stagingDir
    }

    function Copy-SinglePackageMatch {
        param([string]$Directory, [string]$Filter)
        $matches = @(Get-ChildItem -Path $Directory -Filter $Filter -File)
        if ($matches.Count -ne 1) {
            throw "Expected exactly one '$Filter' in $Directory, found $($matches.Count)."
        }
        Copy-Item $matches[0].FullName -Destination $stagingDir
    }

    try {
        New-Item -ItemType Directory -Force -Path $stagingDir | Out-Null

        # SDK binaries required by consumers.
        Copy-PackageFile (Join-Path $packageDir 'openapi-cpp-sdk.dll')
        Copy-PackageFile (Join-Path $packageDir 'openapi-cpp-sdk.lib')

        # Runtime dependencies. Exclude test executables, symbols, linker
        # intermediates, protoc, and protobuf-lite from SDK packages.
        Copy-PackageFile (Join-Path $vcpkgBinDir 'abseil_dll.dll')
        Copy-PackageFile (Join-Path $vcpkgBinDir "cpprest_2_10${debugSuffix}.dll")
        Copy-PackageFile (Join-Path $vcpkgBinDir 'legacy.dll')
        Copy-SinglePackageMatch -Directory $vcpkgBinDir -Filter 'libcrypto-3*.dll'
        Copy-PackageFile (Join-Path $vcpkgBinDir "libprotobuf${debugSuffix}.dll")
        Copy-SinglePackageMatch -Directory $vcpkgBinDir -Filter 'libssl-3*.dll'

        # Import libraries needed to compile and link a consumer application.
        Copy-PackageFile (Join-Path $vcpkgLibDir 'abseil_dll.lib')
        Copy-PackageFile (Join-Path $vcpkgLibDir "cpprest_2_10${debugSuffix}.lib")
        Copy-PackageFile (Join-Path $vcpkgLibDir 'libcrypto.lib')
        Copy-PackageFile (Join-Path $vcpkgLibDir "libprotobuf${debugSuffix}.lib")
        Copy-PackageFile (Join-Path $vcpkgLibDir 'libssl.lib')

        Remove-Item $zipPath -Force -ErrorAction SilentlyContinue
        Compress-Archive -Path (Join-Path $stagingDir '*') -DestinationPath $zipPath -Force
    }
    finally {
        Remove-Item $stagingDir -Recurse -Force -ErrorAction SilentlyContinue
    }

    $zipItem = Get-Item $zipPath
    Write-Host ("    {0}\{1}.zip  {2:N0} bytes" -f $BuildPlatform, $BuildConfiguration, $zipItem.Length)
}

Set-Location $RepoRoot

Write-Host '=========================================='
Write-Host '  Tiger OpenAPI C++ SDK - Windows Package'
Write-Host '=========================================='

if (-not (Test-Path $MSBuildPath)) {
    throw "MSBuild not found: $MSBuildPath"
}

$projectPath = Join-Path $RepoRoot 'openapi-cpp-sdk.vcxproj'
$boostSourceRoot = Join-Path $RepoRoot '.deps\src\boost_1_86_0'
$outputRoot = Join-Path $RepoRoot 'output\Windows'

if (-not (Test-Path $projectPath)) { throw "Project not found: $projectPath" }
if (-not (Test-Path (Join-Path $boostSourceRoot 'boost\asio\io_context.hpp'))) {
    throw "Boost 1.86.0 source headers not found: $boostSourceRoot"
}

if (-not $SkipBuild) {
    Write-Step 'Building SDK configurations...'
    foreach ($buildPlatform in $Platform) {
        $boostTriplet = Get-BoostTriplet -BuildPlatform $buildPlatform
        $boostInstallRoot = Join-Path $RepoRoot ".deps\install\$boostTriplet\boost-1.86.0"

        foreach ($buildConfiguration in $Configuration) {
            Write-Host "  - $buildPlatform $buildConfiguration"
            Initialize-BoostVariant `
                -BuildPlatform $buildPlatform `
                -BuildConfiguration $buildConfiguration `
                -BoostSourceRoot $boostSourceRoot `
                -BoostInstallRoot $boostInstallRoot

            Invoke-SdkBuild `
                -BuildPlatform $buildPlatform `
                -BuildConfiguration $buildConfiguration `
                -ProjectPath $projectPath `
                -BoostSourceRoot $boostSourceRoot `
                -BoostInstallRoot $boostInstallRoot
        }
    }
}

Write-Step 'Creating zip packages...'
foreach ($buildPlatform in $Platform) {
    foreach ($buildConfiguration in $Configuration) {
        New-ZipPackage `
            -BuildPlatform $buildPlatform `
            -BuildConfiguration $buildConfiguration `
            -OutputRoot $outputRoot `
            -RepositoryRoot $RepoRoot
    }
}

Write-Host ''
Write-Host '=========================================='
Write-Host '  Package Complete!'
Write-Host '=========================================='
Write-Host ''
Get-ChildItem -Path $outputRoot -Recurse -Filter '*.zip' |
    Where-Object { $_.DirectoryName -match '\\(Win32|x64)$' } |
    Sort-Object FullName |
    Select-Object FullName, Length, LastWriteTime |
    Format-Table -AutoSize