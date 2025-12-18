#!/usr/bin/env python3
import os
import sys

OUTFILE = "./cgi_dump.txt"

# --- Write headers for the HTTP response ---
sys.stdout.write("Content-Type: text/plain\r\n\r\n")

sys.stdout.write("CGI DEBUG OUTPUT\n")
sys.stdout.write("================\n\n")
sys.stdout.write(f"Writing detailed dump to: {OUTFILE}\n\n")
sys.stdout.flush()

# --- Dump environment variables into file ---
with open(OUTFILE, "w") as f:
    f.write("=== CGI ENVIRONMENT ===\n")
    for key, value in sorted(os.environ.items()):
        f.write(f"{key}={value}\n")

    f.write("\n=== CGI REQUEST BODY ===\n")

    # --- Append request body to file ---
    for line in sys.stdin:
        f.write(line)

# --- Also print a minimal response body ---
sys.stdout.write(f"Environment and body written to {OUTFILE}\n")
sys.stdout.flush()
