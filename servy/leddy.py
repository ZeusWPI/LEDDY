from machine import UART

uart = UART(1)

def scrolling_text(text):
    try:
        print(1)
        uart.init(baudrate=9600, tx=1, rx=None)
        print(2)
        uart.write(text)
        print(3)
    except Exception as e:
        print(e)
        raise Exception('Something went wrong while communicating over serial')
    return f"Displaying scrolling text \"{text}\""

def clear_display():
    # TODO
    return f"Cleared display"

