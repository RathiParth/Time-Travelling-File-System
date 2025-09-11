#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Compiling the Time-Travelling File System..."

g++ -std=c++17 -Wall \
    main.cpp \
    File/FileSystem.cpp \
    -o filesystem

echo "Compilation finished. Executable 'filesystem' created."
