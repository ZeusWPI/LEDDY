# SPDX-License-Identifier: AGPL-3.0-or-later
# SPDX-FileCopyrightText: Copyright 2022-2025 Zeus WPI

import machine
import time
import uos

uos.dupterm(None, 1)
uart = machine.UART(0, baudrate=9600)

retries = 50
retry_interval_ms = 100
expected = b'RX_OK'

def try_write(msg):
    if type(msg) is str:
        msg = msg.encode()
    msg = b'\n' + msg + b'\n'
    success = False
    for i in range(retries):
        if success:
            break
        print('tx attempt', i)
        try:
            uart.write(msg)
            uart.flush()
            while not uart.txdone():
                time.sleep_ms(1)
            timeout_timestamp_ms = time.ticks_ms() + retry_interval_ms
            buf = b''
            while time.ticks_ms() < timeout_timestamp_ms:
                if uart.any():
                    buf += uart.read()
                if buf.find(expected) >= 0:
                    print('tx success')
                    success = True
                    break
        except Exception as e:
            print(e)
    if not success:
        raise Exception(f'Failed writing over uart to leddy, tried {retries} times.')

def text(text):
    try_write(f'T{str(text)}')
    try_write(b'MT')
    return f'Displaying text "{text}"'

def scrolling_text(text):
    try_write(f'T{str(text)}')
    try_write(b'MS')
    return f'Displaying scrolling text "{text}"'

def clear_display():
    try_write(b'MC')
    return 'Cleared display'

def fill_display():
    try_write(b'MF')
    return 'Filled display'

def audio():
    try_write(b'MA')
    return 'Enabled audio'

def option(option):
    try_write(f'O{str(option)}'.encode())
    return f'Set Option "{option}"'
