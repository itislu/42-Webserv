import http.client
import time

HOST = "localhost"
PORT = 8080
PATH = "/cgi-bin/mirror_delay.sh"

chunk1 = b"A" * 5000
chunk2 = b"B" * 5000
body = chunk1 + chunk2

headers = {
    "Content-Type": "application/octet-stream",
    "Content-Length": str(len(body)),
    "Connection": "keep-alive",
}

conn = http.client.HTTPConnection(HOST, PORT)

for i in range(1, 6):
    print(f"\n=== Test {i} ===")

    conn.putrequest("POST", PATH)
    for k, v in headers.items():
        conn.putheader(k, v)
    conn.endheaders()

    conn.send(chunk1)
    print("Sent first 5000 bytes")

    time.sleep(5)

    conn.send(chunk2)
    print("Sent second 5000 bytes")

    response = conn.getresponse()
    response_body = response.read()

    if response_body == body:
        print("SUCCESS: Response matches request body (1:1)")
    else:
        print("FAILURE: Response does NOT match request body")
        print(f"Expected {len(body)} bytes, got {len(response_body)} bytes")

    # small pause between requests on same connection
    time.sleep(1)

conn.close()
print("\nConnection closed")
