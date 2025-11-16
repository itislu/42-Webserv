#!/usr/bin/env bash

# TODO Brief description.

RED='\033[0;31m'
GREEN='\033[0;32m'
RESET='\033[0m'

LOG_FILE=$1

if [ -z "$LOG_FILE" ]; then
    printf "%b" "${RED}Error: no log file specified${RESET}\n"
    exit 1
fi

if [ ! -f "$LOG_FILE" ]; then
    printf "%b" "${RED}Error: file not found: $LOG_FILE${RESET}\n"
    exit 1
fi

# Start capturing when we see a RUN line.
# Continue storing lines while capturing.
# When we find a FAILED line, output all stored lines and reset.
stored_lines=()
capturing=false
num_errors=0
while IFS= read -r line; do
    if [[ "$line" == *" RUN "* ]]; then
        capturing=true
        stored_lines=("$line")
    elif [ "$capturing" = true ]; then
        stored_lines+=("$line")
    fi
    
    if [[ "$line" == *" FAILED "*"ms)" ]]; then
        printf "%s\n" "${stored_lines[@]}" | grep --color=always -E ' FAILED |$'
        if [ $num_errors -gt 0 ]; then
            echo "---"
        fi
        stored_lines=()
        capturing=false
        num_errors=$(( num_errors + 1 ))
    fi
done < "$LOG_FILE"

if [ $num_errors -eq 0 ]; then
    printf "%b" "${GREEN}No failed tests detected.${RESET}\n"
else
    exit 2
fi
