#!/bin/bash

# Navigate to the build directory
cd build || { echo "Build directory not found!"; exit 1; }

# Remove all files in the build directory
rm -rf *

# Run CMake configuration
cmake ..

# Build the project
cmake --build .
