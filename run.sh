#!/bin/sh
# rm -rf build
rm build/output.o build/program.out
./build.sh

./build/marbl_app examples/hello.mrbl
g++ build/output.o -o build/program.out
./build/program.out
echo "exit code: $?"
