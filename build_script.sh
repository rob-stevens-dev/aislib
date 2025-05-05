#!/bin/bash
# Build script for AISLIB

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
cmake .. -DAISLIB_BUILD_TESTS=ON -DAISLIB_BUILD_EXAMPLES=ON

# Build the library, tests and examples
cmake --build . --parallel $(nproc)

# Run the tests
ctest -V

echo ""
echo "Build completed. If successful, you can find:"
echo "- Library in: $(pwd)/libaislib.a"
echo "- Test executables in: $(pwd)"
echo "- Example executables in: $(pwd)"
echo ""
echo "To run the multi-part message example:"
echo "$(pwd)/multipart_example"