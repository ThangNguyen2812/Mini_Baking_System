@echo off
REM Build script for Mini Banking System

echo Building Mini Banking System...
echo.

cd /d "%~dp0"

REM Compile with organized structure
g++ -std=c++17 -I./include source/*.cpp -o banking_system.exe

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ===== BUILD SUCCESSFUL =====
    echo Executable: banking_system.exe
    echo Location: %cd%
    echo.
    echo To run the system:
    echo   .\banking_system.exe
) else (
    echo.
    echo ===== BUILD FAILED =====
    echo Check compilation errors above.
    pause
)
