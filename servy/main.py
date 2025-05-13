import gc
import leddy
import machine
import network
import socket
import time
import uos
import webrepl

def connect_wifi() -> None:
    # Disable access point
    interface = network.WLAN(network.AP_IF)
    interface.active(False)
    # Connect to Zeusnet
    interface = network.WLAN(network.STA_IF)
    print('Connecting to Zeus WPI network ...')
    interface.active(True)
    interface.connect('Zeus WPI', 'zeusisdemax')
    while not interface.isconnected():
        print('Connecting...')
        time.sleep(1)
    print('Network IP:', interface.ifconfig()[0])

def start_webrepl() -> None:
    # Start webrepl on port 8266
    webrepl.start()

def main() -> None:
    s = listen()
    while True:
        conn, addr = s.accept()
        print()
        print(f'Client connected from {addr[0]}')
        handle_connection(conn)
        conn.close()

def listen() -> None:
    addr = socket.getaddrinfo('0.0.0.0', 80)[0][-1]
    s = socket.socket()
    s.bind(addr)
    s.listen(4)
    print('Listening on', addr)
    return s

def handle_connection(conn) -> None:
    try:
        first_line = conn.readline().decode('utf-8').strip()
        print(f'Request: {first_line}')

        first_line_split = first_line.split(' ')

        if len(first_line_split) < 3:
            raise Exception(f'Bad first line "{first_line}"')

        method = first_line_split[0]
        if method not in {'GET', 'POST'}:
            raise Exception(f'Unsupported HTTP method "{method}"')

        # Read headers
        while True:
            line = conn.readline()
            if not line or line in [b'\n', b'\r\n']:
                break

        if method == 'GET':
            # Send index.html
            body = read_file('index.html').encode('utf-8')
            conn.sendall('HTTP/1.0 200 OK\r\n')
            conn.sendall('Content-type: text/html\r\n')
            conn.sendall(f'Content-length: {len(body)}\r\n')
            conn.sendall('\r\n')
            conn.sendall(body)
        elif method == 'POST':
            # Read body
            req_body = None
            try:
                conn.settimeout(0) # If no body at all, will not wait for one
                req_body = conn.readline().decode('utf-8').strip()
            except:
                raise Exception('No body')
            if not req_body: # Could be empty after strip
                raise Exception(f'Empty body')
            print(f'Body: {req_body}')

            # Process body
            res_body = process_body(req_body)
            print(f'Action taken: {res_body}')

            # Send what was done
            res_body = res_body.encode('utf-8')
            conn.sendall('HTTP/1.0 200 OK\r\n')
            conn.sendall('Content-type: text/plain\r\n')
            conn.sendall(f'Content-length: {len(res_body)}\r\n')
            conn.sendall('\r\n')
            conn.sendall(res_body)
        else:
            assert False, "Unreachable code"
    except Exception as e:
        print(f'Error occured: {e}')

        body = f'Error: {e}\n'
        body += read_file('usage.txt')
        body = body.encode('utf-8')

        conn.sendall('HTTP/1.0 400 Bad request :(\r\n')
        conn.sendall('Content-type: text/plain\r\n')
        conn.sendall(f'Content-length: {len(body)}\r\n')
        conn.sendall('\r\n')
        conn.sendall(body)

def read_file(file_name: str) -> str:
    f = open(file_name, 'r')
    contents = f.read()
    f.close()
    return contents

def process_body(body: str) -> str:
    command = body.split(' ')[0]
    args = body[len(command)+1:]
    if command == 'Text':
        if args:
            return leddy.text(args)
        raise Exception('No text to display')
    elif command == 'ScrollingText':
        if args:
            return leddy.scrolling_text(args)
        raise Exception('No text to display')
    elif command == 'ClearDisplay':
        return leddy.clear_display()
    elif command == 'FillDisplay':
        return leddy.fill_display()
    elif command == 'Audio':
        return leddy.audio()
    elif command == 'Option':
        return leddy.option(args)
    else:
        raise Exception(f'No such command: "{command}"')

connect_wifi()
start_webrepl()
main()
