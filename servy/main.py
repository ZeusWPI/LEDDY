import machine

import leddy

import socket
addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
s = socket.socket()
s.bind(addr)
s.listen(4)
print('[MAIN] Listening on', addr)

USAGE = """

---
Usage: HTTP Post with the following plaintext body:

COMMAND [command opts ...]

Commands:
    ScrollingText <text>
    ClearDisplay
    FillDisplay
---
"""

index_file = open("index.html", "r")
INDEX_HTML = index_file.read()
index_file.close()


def process_body(body: str) -> str:
    command = body.split(' ')[0]
    if command == "ScrollingText":
        text = body[len(command)+1:] # +1 for space
        if text:
            return leddy.scrolling_text(text)
        raise Exception("No text to display")
    elif command == "ClearDisplay":
        return leddy.clear_display()
    elif command == "FillDisplay":
        return leddy.fill_display()

    raise Exception(f"No such command: \"{command}\"")


# Main loop that handles requests
while True:
    conn, addr = s.accept()
    print(f'\n[MAIN] Client connected from {addr[0]}')

    try:
        line = conn.readline()
        print(f'[MAIN] Request: {line}')
        # POST request (to root)
        if line[:7] == b'POST / ' or line[:7] == b'POST /?':
            # Read headers
            while line and line not in [b'\n', b'\r\n']:
                line = conn.readline()

            # Read body
            try:
                conn.settimeout(0) # If no body at all, will not wait for one
                body = conn.readline().decode('UTF-8').strip()
            except:
                raise Exception('No body')
            if not body: # Could be empty after strip
                raise Exception(f"Empty body")
            print(f'[MAIN] Body: {body}')

            # Process body
            reply = process_body(body)
            print(f"[MAIN] Action taken: {reply}")

            # Send what was done
            conn.send('HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n')
            conn.send(reply)
        # GET request (to root)
        elif line[:6] == b'GET / ' or line[:6] == b'GET /?':
            # Read headers
            while line and line != b'\r\n':
                line = conn.readline()

            # Send index.html
            conn.send('HTTP/1.0 200 OK\r\nContent-type: text/html\r\n\r\n')
            conn.sendall(INDEX_HTML)
        # Other request
        else:
            # Read headers (otherwise 'Connection ended')
            while line and line not in [b'\n', b'\r\n']:
                line = conn.readline()
            raise Exception("Not supported")
    except Exception as e:
        print(f"[MAIN] Error occured: {e}")
        conn.send('HTTP/1.0 400 Bad request :(\r\nContent-type: text/plain\r\n\r\n')
        conn.send(f"Error: {e}{USAGE}")

    conn.close()

