#!/bin/bash
# A script to compile the Time-Travelling File System project.
# It compiles all necessary.cpp files and links them into a single executable.

echo "Compiling the Time-Travelling File System..."

g++ -std=c++17 -Wall -o filesystem main.cpp File/File.cpp File/FileSystem.cpp

echo "Compilation finished. Executable 'filesystem' created."