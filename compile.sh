#!/bin/bash

# Exit immediately if a command fails
set -e

echo "Compiling the Time-Travelling File System..."

# The g++ command that compiles and links all necessary source files.
# We have added File/File.cpp to the list.
g++ -std=c++17 -Wall \
    main.cpp \
    File/FileSystem.cpp \
    File/File.cpp \
    -o filesystem

echo "Compilation finished. Executable 'filesystem' created."