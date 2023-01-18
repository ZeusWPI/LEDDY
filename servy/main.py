import machine

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
---
"""

def process_body(body):
    command = body.split(' ')[0]
    if command == "ScrollingText":
        text = body[len(command)+1:] # +1 for space
        if text:
            # TODO
            return f"Displaying scrolling text \"{text}\""
        raise Exception("No text to display")
    elif command == "ClearDisplay":
        return f"Cleared display"

    raise Exception(f"No such command: \"{command}\"")


while True:
    conn, addr = s.accept()
    print('\n[MAIN] client connected from ', addr)

    try:
        line = conn.readline()
        print(f'[MAIN] Request: {line}')
        if line[:6] == b'POST /':
            # Read headers
            while line and line != b'\r\n':
                line = conn.readline()

            # Read body
            try:
                conn.settimeout(0.1)
                body = conn.readline()
            except:
                raise Exception('No body')
            if body:
                body = body.decode('UTF-8').strip()
            if not body: # Could be empty after strip
                raise Exception(f"Empty body")
            print(f'[MAIN] Body: {body}')

            # Process body
            reply = process_body(body)

            # Reply
            conn.send('HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n')
            conn.send(reply)
        else:
            # Read headers
            while line and line != b'\r\n':
                line = conn.readline()

            raise Exception("Only POST is not implemented")
    except Exception as e:
        print(f"[MAIN] Error occured: {e}")
        conn.send('HTTP/1.0 400 Bad request :(\r\nContent-type: text/plain\r\n\r\n')
        conn.send(f"Error: {e}{USAGE}")

    conn.close()

