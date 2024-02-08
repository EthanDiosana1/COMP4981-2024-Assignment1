#!/bin/bash

# Step 1: Run ./generate-cmakelists.sh
./generate-cmakelists.sh

# Step 2: Run ./change-compiler.sh -c clang
./change-compiler.sh -c clang

# Step 3: Run ./build.sh
./build.sh
