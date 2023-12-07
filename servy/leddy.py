import machine

uart = machine.UART(0, baudrate=9600)

def text(text):
    try:
        uart.write(f"T{str(text)}\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Displaying text \"{text}\""

def scrolling_text(text):
    try:
        uart.write(f"S{str(text)}\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Displaying scrolling text \"{text}\""

def clear_display():
    try:
        uart.write("UClearDisplay\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return "Cleared display"

def fill_display():
    try:
        uart.write("UFillDisplay\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return "Filled display"

def audio():
    try:
        uart.write("A\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return "Enabled audio"

def option(option):
    try:
        uart.write(f"O{str(option)}\n")
    except Exception as e:
        raise Exception('Something went wrong while communicating over serial')
    return f"Set Option {option}"
