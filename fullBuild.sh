#!/bin/bash

./generate-flags.sh

./generate-cmakelists.sh

./change-compiler.sh -c clang

./build.sh

cp -r ./html ./build
