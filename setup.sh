#!/bin/bash

set -e

# Detect OS
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
else
    echo "Unsupported OS"
    exit 1
fi

echo "Setting up for $OS..."

# Install Homebrew if not present
if ! command -v brew &> /dev/null; then
    echo "Installing Homebrew..."
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

if [ "$OS" = "macos" ]; then
    # Homebrew packages
    brew install \
        git \
        python@3.11 \
        node \
        cmake \
        nlohmann-json \
        zeromq

    brew tap homebrew/cask

elif [ "$OS" = "linux" ]; then
    # Update package manager
    sudo apt-get update
    sudo apt-get upgrade -y

    # APT packages
    sudo apt-get install -y \
        git \
        python3 \
        python3-pip \
        nodejs \
        npm \
        cmake \
        build-essential \
        nlohmann-json3-dev

    # Install ZeroMQ with Homebrew
    brew install zeromq
fi

echo "Setup complete!"