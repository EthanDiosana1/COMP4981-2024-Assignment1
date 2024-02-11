#!/bin/bash

./generate-cmakelists.sh

./change-compiler.sh -c clang

./build.sh

cp -r ./server_files ./build