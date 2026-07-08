# PRISM build driver (PowerShell).
#
# Composes a complete MSVC environment deterministically from the installed
# toolset + Windows SDK folders (robust against flaky vswhere/DevShell), puts
# CMake + Ninja on PATH, then configures and builds a Release VST3 + Standalone.
# First run clones JUCE (a few minutes).

$ErrorActionPreference = 'Stop'
$root = Split-Path -Parent $MyInvocation.MyCommand.Path

function Find-Latest($base) {
    if (-not (Test-Path $base)) { return $null }
    $d = Get-ChildItem $base -Directory -ErrorAction SilentlyContinue | Sort-Object Name -Descending | Select-Object -First 1
    if ($d) { return $d.FullName } else { return $null }
}

# --- 1) MSVC toolset (must have the STL/CRT headers) ---
$msvcRoots = @(
    "C:\Program Files\Microsoft Visual Studio\18\Insiders\VC\Tools\MSVC",
    "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\MSVC",
    "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\MSVC",
    "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC"
)
$msvc = $null
foreach ($r in $msvcRoots) {
    $m = Find-Latest $r
    if ($m -and (Test-Path (Join-Path $m 'include\vcruntime.h'))) { $msvc = $m; break }
}
if (-not $msvc) { throw "No complete MSVC toolset (with STL headers) found. Install the 'Desktop development with C++' workload." }

# --- 2) Windows SDK ---
$sdkRoot = @("C:\Program Files (x86)\Windows Kits\10", "C:\Program Files\Windows Kits\10") |
           Where-Object { Test-Path (Join-Path $_ 'Include') } | Select-Object -First 1
if (-not $sdkRoot) { throw "Windows 10/11 SDK not found." }
$sdkVer = (Get-ChildItem (Join-Path $sdkRoot 'Include') -Directory -ErrorAction SilentlyContinue |
           Where-Object { (Test-Path (Join-Path $_.FullName 'ucrt\stdio.h')) -and
                          (Test-Path (Join-Path $_.FullName 'um\windows.h')) } |
           Sort-Object Name -Descending | Select-Object -First 1).Name
if (-not $sdkVer) { throw "No usable Windows SDK version (ucrt + um headers) found." }

# --- 3) compose environment ---
$env:INCLUDE = @(
    (Join-Path $msvc 'include'),
    (Join-Path $sdkRoot "Include\$sdkVer\ucrt"),
    (Join-Path $sdkRoot "Include\$sdkVer\shared"),
    (Join-Path $sdkRoot "Include\$sdkVer\um"),
    (Join-Path $sdkRoot "Include\$sdkVer\winrt")
) -join ';'
$env:LIB = @(
    (Join-Path $msvc 'lib\x64'),
    (Join-Path $sdkRoot "Lib\$sdkVer\ucrt\x64"),
    (Join-Path $sdkRoot "Lib\$sdkVer\um\x64")
) -join ';'
$env:WindowsSdkDir = "$sdkRoot\"
$env:WindowsSDKVersion = "$sdkVer\"
$env:VCToolsInstallDir = "$msvc\"

$clBin  = Join-Path $msvc 'bin\Hostx64\x64'
$sdkBin = Join-Path $sdkRoot "bin\$sdkVer\x64"
$env:PATH = "$clBin;$sdkBin;$env:PATH"

# CMake — prefer PATH, else the standard install location
if (-not (Get-Command cmake -ErrorAction SilentlyContinue)) {
    $cm = 'C:\Program Files\CMake\bin'
    if (Test-Path (Join-Path $cm 'cmake.exe')) { $env:PATH = "$cm;$env:PATH" }
}
# Ninja — prefer PATH, else the winget-installed copy
if (-not (Get-Command ninja -ErrorAction SilentlyContinue)) {
    $nj = Get-ChildItem "$env:LOCALAPPDATA\Microsoft\WinGet\Packages" -Recurse -Filter ninja.exe -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($nj) { $env:PATH = "$(Split-Path $nj.FullName);$env:PATH" }
}

Write-Host "[PRISM] MSVC : $msvc"
Write-Host "[PRISM] SDK  : $sdkRoot ($sdkVer)"
Write-Host "[PRISM] cl   : $((Get-Command cl -ErrorAction SilentlyContinue).Source)"

# --- 4) configure + build ---
Set-Location $root
Write-Host "[PRISM] Configuring (first run clones JUCE)..."
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
if ($LASTEXITCODE -ne 0) { throw "configure failed" }

Write-Host "[PRISM] Building..."
cmake --build build
if ($LASTEXITCODE -ne 0) { throw "build failed" }

Write-Host "[PRISM] BUILD COMPLETE"
