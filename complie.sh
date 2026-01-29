#!/bin/bash

# Make this script executable with: chmod u+x build.sh

echo "Compiling Suspension Control System..."

# Homebrew paths (works on both Intel & Apple Silicon)
# brew install zeromq cppzmq
INCLUDE_PATHS="-I/opt/homebrew/include -I/usr/local/include"
LIB_PATHS="-L/opt/homebrew/lib -L/usr/local/lib"

# see if ./build exists
if [ -d "build" ]; then
    :
else
    mkdir build
fi

# see if ./logs exists
if [ -d "logs" ]; then
    :
else
    mkdir logs
fi

# Compile and link
c++ -std=c++17 \
    src/*.cpp \
    $INCLUDE_PATHS \
    $LIB_PATHS \
    -lzmq \
    -pthread \
    -o build/SemiActiveWrapper

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build complete. Running..."
    echo "-----------------------------"
    ./build/SemiActiveWrapper
else
    echo "Build failed. Please check for errors above."
fi
