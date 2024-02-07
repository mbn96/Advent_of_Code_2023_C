#!/usr/bin/bash

SRC_FILE=$1
if [[ -z $SRC_FILE ]]; then
  echo "Usage: $0 <source file>"
  exit 1
fi
if [[ ! -f $SRC_FILE ]]; then
  echo "Error: $SRC_FILE does not exist"
  exit 1
fi

SHARED_FILES=$(find . -maxdepth 1 -name "*.c")
SRC_FILES="$SRC_FILE $SHARED_FILES"

echo "Compiling: $SRC_FILES"
OUTPUT_NAME="$(basename $SRC_FILE .c)_$(date +%Y-%m-%d_%H.%M.%S)"
mkdir -p build
clang -Wall -Werror -Wpedantic -std=c11 -g -O0 -o build/$OUTPUT_NAME $SRC_FILES
