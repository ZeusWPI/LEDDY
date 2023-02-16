import machine

uart = machine.UART(0, baudrate=9600)

def text(text):
    try:
        uart.write(f"2{str(text)}\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Displaying text \"{text}\""

def scrolling_text(text):
    try:
        uart.write(f"1{str(text)}\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Displaying scrolling text \"{text}\""

def clear_display():
    try:
        uart.write("0ClearDisplay\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Cleared display"

def fill_display():
    try:
        uart.write("0FillDisplay\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Filled display"

