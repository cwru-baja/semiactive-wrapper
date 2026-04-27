#!/bin/bash

# see if ./logs exists
if [ -d "logs" ]; then
    :
else
    mkdir logs
fi

# Check if the build exists
if [ -f "./build/SemiActiveWrapper" ]; then
    echo "Running Suspension Control System..."
    echo "-----------------------------"
    # make run reletive to this file, regardless of where the user is running it from
    cd ~/piM/semiactive-wrapper/build
    ./SemiActiveWrapper
else
    echo "Build not found. Please run ./compile.sh first to build the project."
fi
