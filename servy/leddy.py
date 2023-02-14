import machine

uart = machine.UART(0, baudrate=9600)

def scrolling_text(text):
    try:
        uart.write(text + "\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Displaying scrolling text \"{text}\""

def clear_display():
    # TODO
    return f"Cleared display"

