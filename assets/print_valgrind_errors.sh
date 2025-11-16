#!/usr/bin/env bash

# TODO Brief description.

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
RESET='\033[0m'

# Get all arguments as separate words (recommended)
LOG_FILES=("$@")

if [ ${#LOG_FILES[@]} -eq 0 ]; then
    printf "%b" "${RED}Error: no log files specified${RESET}\n"
    exit 1
fi

if ! ls "${LOG_FILES[@]}" &>/dev/null; then
    printf "%b" "${RED}Error: no files found${RESET}\n"
    exit 2
fi

pattern='at 0x[[:xdigit:]]+:'
num_errors=0
for file in "${LOG_FILES[@]}"; do
    if grep --quiet -E "$pattern" "$file"; then
        printf "%b" "\n${YELLOW}--- $file ---${RESET}\n"
        grep --color=always -E "$pattern|$" "$file"
        num_errors=$(( num_errors + 1 ))
    fi
done

if [ $num_errors -eq 0 ]; then
    printf "%b" "${GREEN}No valgrind issues detected.${RESET}\n"
else
    exit 3
fi
