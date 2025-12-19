#!/bin/sh

# --- Write headers for the HTTP response ---
printf "Content-Type: text/plain\r\n"
printf "\r\n"

sleep 10

printf "42\n"
