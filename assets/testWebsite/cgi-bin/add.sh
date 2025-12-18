#!/bin/bash

printf "Content-Type: text/plain\r\n"
printf "\r\n"

# Parse query string
IFS='&' read -r -a params <<< "$QUERY_STRING"

for p in "${params[@]}"; do
  key=$(echo "$p" | cut -d= -f1)
  val=$(echo "$p" | cut -d= -f2)

  case "$key" in
    a) A=$val ;;
    b) B=$val ;;
  esac
done

# Default to 0 if empty
A=${A:-0}
B=${B:-0}

RESULT=$((A + B))

echo "A = $A"
echo "B = $B"
echo "Result = $RESULT"
