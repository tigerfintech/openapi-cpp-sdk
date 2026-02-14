# Build Release-MT configurations with abseil library path injection
# Usage: .\build_release_mt.ps1 [-Platform <Win32|x64>] [-Clean]

param(
    [Parameter()]
    [ValidateSet("Win32", "x64")]
    [string]$Platform = "x64",
    
    [Parameter()]
    [switch]$Clean
)

$ErrorActionPreference = "Stop"

# Paths
$ProjectRoot = $PSScriptRoot
$ProjectFile = Join-Path $ProjectRoot "openapi-cpp-sdk.vcxproj"
$DepsPath = Join-Path $ProjectRoot "deps\Windows\$Platform\Release-MT"
$MSBuildPath = "C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe"

# Verify paths
if (-not (Test-Path $ProjectFile)) {
    Write-Error "Project file not found: $ProjectFile"
    exit 1
}

if (-not (Test-Path $MSBuildPath)) {
    Write-Error "MSBuild not found: $MSBuildPath"
    exit 1
}

if (-not (Test-Path $DepsPath)) {
    Write-Warning "Deps path not found: $DepsPath"
    Write-Host "Creating directory and copying abseil libraries..."
    New-Item -ItemType Directory -Path $DepsPath -Force | Out-Null
    
    # Copy abseil libs from vcpkg
    $VcpkgLibPath = Join-Path $ProjectRoot "vcpkg_installed_static\$Platform-windows-static\lib"
    if (Test-Path $VcpkgLibPath) {
        Copy-Item -Path "$VcpkgLibPath\absl_*.lib" -Destination $DepsPath -Force
        Write-Host "Copied abseil libraries to $DepsPath"
    } else {
        Write-Error "vcpkg static libraries not found: $VcpkgLibPath"
        exit 1
    }
}

# Set LIB environment variable to include deps path
$env:LIB = "$env:LIB;$DepsPath"
Write-Host "LIB path injected: $DepsPath" -ForegroundColor Green

# Determine build target
$BuildTarget = if ($Clean) { "Rebuild" } else { "Build" }

# Build
Write-Host "`nBuilding Release-MT $Platform ($BuildTarget)..." -ForegroundColor Cyan
Write-Host "Command: $MSBuildPath" -ForegroundColor Gray
Write-Host "  Project: $ProjectFile" -ForegroundColor Gray
Write-Host "  Target: $BuildTarget" -ForegroundColor Gray
Write-Host "  Configuration: Release-MT" -ForegroundColor Gray
Write-Host "  Platform: $Platform`n" -ForegroundColor Gray

& $MSBuildPath $ProjectFile `
    /t:$BuildTarget `
    /p:Configuration=Release-MT `
    /p:Platform=$Platform `
    /m:1 `
    /v:minimal

if ($LASTEXITCODE -eq 0) {
    Write-Host "`nBuild succeeded!" -ForegroundColor Green
} else {
    Write-Error "Build failed with exit code $LASTEXITCODE"
    exit $LASTEXITCODE
}
