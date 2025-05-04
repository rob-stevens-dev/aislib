#!/bin/bash
# Script to create symbolic links for consistent file naming

# Change to script directory
cd "$(dirname "$0")"

# Get the full path to the include directory
INCLUDE_DIR="$(pwd)/include/aislib"
echo "Include directory: $INCLUDE_DIR"

# Create symbolic link from static_data.h to static_and_voyage_data.h if needed
if [ -f "$INCLUDE_DIR/static_data.h" ]; then
    echo "Link for static_data.h already exists"
else
    if [ -f "$INCLUDE_DIR/static_and_voyage_data.h" ]; then
        echo "Creating link from static_data.h to static_and_voyage_data.h"
        cd "$INCLUDE_DIR"
        ln -sf static_and_voyage_data.h static_data.h
        echo "Link created successfully"
    else
        echo "Error: source file static_and_voyage_data.h does not exist"
        echo "Please make sure to create this file first"
    fi
fi

echo "Symlink creation completed"