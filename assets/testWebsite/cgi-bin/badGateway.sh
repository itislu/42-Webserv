#!/bin/bash

# Read entire POST body
BODY="$(head -c "$CONTENT_LENGTH")"

# Required CGI header
printf "Content-Type: application/octet-stream\r\n"
printf "wrongColon : somevalue\r\n"
printf "\r\n"

# Wait 5 seconds
sleep 5

OUTFILE="./cgi_dump.txt"
echo "$BODY" > "$OUTFILE"

# Echo body back 1:1
printf "%s" "$BODY"
