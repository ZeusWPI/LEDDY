# This file is executed on every boot (including wake-boot from deepsleep)
#import esp
#esp.osdebug(None)
import uos, machine
uos.dupterm(None, 1) # disable REPL on UART(0)
import gc

import network

# Disable access point
interface = network.WLAN(network.AP_IF)
interface.active(False)

# Enable connecting to Zeusnet
interface = network.WLAN(network.STA_IF)
print('[BOOT] Connecting to Zeus WPI network ...')
interface.active(True)
interface.connect('Zeus WPI', 'zeusisdemax')
while not interface.isconnected():
    pass
print('[BOOT] Network IP:', interface.ifconfig()[0])


import webrepl
webrepl.start()
gc.collect()

