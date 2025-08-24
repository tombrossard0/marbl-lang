#!/bin/sh
# rm -rf build
./build.sh

./build/marbl_app examples/hello.mrbl
clang++ build/output.o -o build/program.out
./build/program.out
echo $?
