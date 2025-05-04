@echo off
REM Script to create copies of header files for consistent naming on Windows

REM Change to script directory
cd /d "%~dp0"

REM Get the full path to the include directory
set "INCLUDE_DIR=%CD%\include\aislib"
echo Include directory: %INCLUDE_DIR%

REM Create copy from static_and_voyage_data.h to static_data.h if needed
if exist "%INCLUDE_DIR%\static_data.h" (
    echo Copy for static_data.h already exists
) else (
    if exist "%INCLUDE_DIR%\static_and_voyage_data.h" (
        echo Creating copy from static_and_voyage_data.h to static_data.h
        copy "%INCLUDE_DIR%\static_and_voyage_data.h" "%INCLUDE_DIR%\static_data.h"
        echo Copy created successfully
    ) else (
        echo Error: source file static_and_voyage_data.h does not exist
        echo Please make sure to create this file first
    )
)

echo Header copy creation completed
pause