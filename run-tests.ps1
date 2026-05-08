<#
Run build and tests for Mini Banking System (Windows PowerShell)

Usage:
  .\run-tests.ps1                # configure (if needed), build, run all tests via ctest
  .\run-tests.ps1 -Configure     # force CMake configure step
  .\run-tests.ps1 -GTestFilter "AccountTest.*"  # run filtered tests

This script tries to use `cmake` and `ctest` on PATH; if not found
it falls back to common MSYS2 locations used in this project.
#>

param(
    [switch]$Configure,
    [string]$GTestFilter = ''
)

function Find-Exe($name, $fallback) {
    $cmd = Get-Command $name -ErrorAction SilentlyContinue
    if ($cmd) { return $cmd.Path }
    if (Test-Path $fallback) { return $fallback }
    return $null
}

$cmake = Find-Exe cmake 'C:\msys64\ucrt64\bin\cmake.exe'
$ctest = Find-Exe ctest 'C:\msys64\ucrt64\bin\ctest.exe'

if (-not $cmake) { Write-Error 'CMake not found on PATH and no fallback available.'; exit 1 }
if (-not $ctest) { Write-Warning 'ctest not found on PATH; will run test binary directly if available.' }

Write-Host "Using CMake: $cmake"

# Configure if requested or if build directory is missing
if ($Configure -or -not (Test-Path 'build-tests\CMakeCache.txt')) {
    Write-Host 'Running CMake configure...'
    & $cmake -S . -B build-tests -G Ninja -DCMAKE_CXX_COMPILER=C:/msys64/ucrt64/bin/g++.exe -DCMAKE_PREFIX_PATH=C:/msys64/ucrt64
    if ($LASTEXITCODE -ne 0) { Write-Error 'CMake configure failed'; exit $LASTEXITCODE }
}

Write-Host 'Building tests...'
& $cmake --build build-tests
if ($LASTEXITCODE -ne 0) { Write-Error 'Build failed'; exit $LASTEXITCODE }

if ($GTestFilter -ne '') {
    $exe = Join-Path 'build-tests' 'bank_tests.exe'
    if (-not (Test-Path $exe)) { Write-Error "Test executable $exe not found"; exit 1 }
    Write-Host "Running tests with filter: $GTestFilter"
    & $exe --gtest_filter=$GTestFilter
    exit $LASTEXITCODE
}

if ($ctest) {
    Write-Host 'Running all tests via ctest...'
    & $ctest --test-dir build-tests --output-on-failure
    exit $LASTEXITCODE
} else {
    $exe = Join-Path 'build-tests' 'bank_tests.exe'
    if (-not (Test-Path $exe)) { Write-Error "Test executable $exe not found and ctest missing"; exit 1 }
    Write-Host 'Running all tests via test binary...'
    & $exe
    exit $LASTEXITCODE
}
