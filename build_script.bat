@echo off
REM Build script for AISLIB on Windows

REM Create build directory if it doesn't exist
if not exist build mkdir build
cd build

REM Configure with CMake
cmake .. -DAISLIB_BUILD_TESTS=ON -DAISLIB_BUILD_EXAMPLES=ON

REM Build the library, tests and examples
cmake --build . --config Release

REM Run the tests
ctest -C Release -V

echo.
echo Build completed. If successful, you can find:
echo - Library in: %CD%\Release\aislib.lib
echo - Test executables in: %CD%\Release
echo - Example executables in: %CD%\Release
echo.
echo To run the multi-part message example:
echo %CD%\Release\multipart_example.exe

cd ..