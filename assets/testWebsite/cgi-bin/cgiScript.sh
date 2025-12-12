#!/bin/sh

OUTFILE="./cgi_dump.txt"

# --- Write headers for the HTTP response ---
printf "Content-Type: text/plain\r\n"
printf "\r\n"

echo "CGI DEBUG OUTPUT" 
echo "================"
echo
echo "Writing detailed dump to: $OUTFILE"
echo

# --- Dump environment variables into file ---
{
    echo "=== CGI ENVIRONMENT ==="
    env | sort
    echo
    echo "=== CGI REQUEST BODY ==="
} > "$OUTFILE"

# --- Append request body to file ---
while IFS= read -r line; do
    echo "$line" >> "$OUTFILE"
done

# --- Also print a minimal response body ---
echo "Environment and body written to $OUTFILE"
