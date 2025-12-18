#!/usr/bin/env python3
import os
import sys
import time
import errno

# --- HTTP response headers ---
sys.stdout.write("Content-Type: text/plain\r\n\r\n")
sys.stdout.flush()

content_length = int(os.environ.get("CONTENT_LENGTH", "0"))
read_bytes = 0
body_parts = []

fd = sys.stdin.fileno()

while read_bytes < content_length:
    try:
        # Read remaining bytes
        remaining = content_length - read_bytes
        data = os.read(fd, remaining)

        if not data:
            # EOF
            break

        body_parts.append(data)
        read_bytes += len(data)

        # Debug
        print(f"CGI_SCRIPT_DEBUG: read = {read_bytes}", file=sys.stderr)

    except OSError as e:
        if e.errno in (errno.EAGAIN, errno.EWOULDBLOCK):
            # No data yet, retry later
            time.sleep(0.1)
            continue
        else:
            raise

# Simulate slow CGI
time.sleep(5)

# Echo body back 1:1
sys.stdout.buffer.write(b"".join(body_parts))
sys.stdout.flush()
