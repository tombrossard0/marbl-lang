#!/bin/bash
set -e # Exit immediately on error

mkdir -p build
cd build

cmake -DCMAKE_BUILD_TYPE=Debug ..

cmake --build .
