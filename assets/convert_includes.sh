#!/bin/bash

# convert_includes.sh
# Converts quoted #include paths containing / into angle-bracket form.

os_name=$(uname)
if [ "$os_name" = "Darwin" ]; then
    echo "Running on macOS"
    sed_inplace() { sed -i '' -E "$@"; }
elif [ "$os_name" = "Linux" ]; then
    echo "Running on Linux"
    sed_inplace() { sed -i -E "$@"; }
else
    echo "Unknown OS: $os_name"
    exit 1
fi

for file in "$@"; do
  echo "Processing $file"
  sed_inplace 's|#include "([^"]*/[^"]*)"|#include <\1>|g' "$file"
done
