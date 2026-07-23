$ErrorActionPreference = "Stop"
$boostVersion = "1.86.0"
$boostToken = $boostVersion -replace "\.", "_"
$repoRoot = Split-Path -Parent $PSScriptRoot
$depsRoot = Join-Path $repoRoot ".deps2"
$boostRoot = Join-Path $depsRoot "boost"
$downloadDir = Join-Path $depsRoot "downloads"
$zipPath = Join-Path $downloadDir ("boost_{0}.zip" -f $boostToken)
$extractDir = Join-Path $boostRoot ("boost_{0}" -f $boostToken)
$uri = "https://archives.boost.io/release/$boostVersion/source/boost_${boostToken}.zip"

New-Item -ItemType Directory -Force -Path $downloadDir | Out-Null
if (Test-Path $boostRoot) {
    Remove-Item -Recurse -Force $boostRoot
}
New-Item -ItemType Directory -Force -Path $boostRoot | Out-Null

Write-Host "Downloading Boost $boostVersion..."
Invoke-WebRequest -Uri $uri -OutFile $zipPath
Write-Host "Extracting to $boostRoot..."
Expand-Archive -Path $zipPath -DestinationPath $boostRoot -Force

if (-not (Test-Path $extractDir)) {
    throw "Expected Boost directory not found: $extractDir"
}
Write-Host "Boost ready at $extractDir"