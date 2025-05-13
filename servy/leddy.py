import machine
import uos

uos.dupterm(None, 1)
uart = machine.UART(0, baudrate=9600)

def try_write(msg):
    try:
        uart.write(msg)
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')

def text(text):
    try_write(f'T{str(text)}\n')
    return f'Displaying text "{text}"'

def scrolling_text(text):
    try_write(f'S{str(text)}\n')
    return f'Displaying scrolling text "{text}"'

def clear_display():
    try_write('UClearDisplay\n')
    return 'Cleared display'

def fill_display():
    try_write('UFillDisplay\n')
    return 'Filled display'

def audio():
    try_write('A\n')
    return 'Enabled audio'

def option(option):
    try_write(f'O{str(option)}\n')
    return f'Set Option {option}'
