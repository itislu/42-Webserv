#!/bin/bash

# convert_includes.sh
# Converts quoted #include paths containing / into angle-bracket form.

for file in "$@"; do
  echo "Processing $file"
  sed -E -i 's|#include "([^"]*/[^"]*)"|#include <\1>|g' "$file"
done
