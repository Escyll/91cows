import bluetooth
import time

target_address = "20:19:05:15:18:26"


# print("performing inquiry...")

# nearby_devices = bluetooth.discover_devices(
#         duration=8, lookup_names=True, flush_cache=True, lookup_class=False)

# print("found %d devices" % len(nearby_devices))

# for addr, name in nearby_devices:
#     try:
#         print("  %s - %s" % (addr, name))
#     except UnicodeEncodeError:
#         print("  %s - %s" % (addr, name.encode('utf-8', 'replace')))

port = 1

sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
sock.connect((target_address, port))
for i in range(0,10):
    sock.send("f:1\r")
    time.sleep(1)
    sock.send("s:0\r")
    time.sleep(1)
sock.close()