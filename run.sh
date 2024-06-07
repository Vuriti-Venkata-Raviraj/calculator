#!/bin/bash

# Compile the C server
cd server/
mkdir -p build
cd build
cmake ..
make

# Run the C server in the background
./server &

# Change to the .NET application directory if needed
cd ../../MyCalculator

# Run the .NET application
dotnet run
