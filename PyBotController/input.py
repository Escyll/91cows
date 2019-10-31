from pynput.keyboard import Key, Listener
import bluetooth
import time

target_address = "20:19:05:15:18:26"

port = 1
sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
sock.connect((target_address, port))

pressed_key = Key.ctrl

def on_press(key):
    global pressed_key
    if (pressed_key == key):
        return
    else:
        pressed_key = key
        if (pressed_key == Key.up):
            print('Driving forward')
            sock.send("f:100\r")
        elif (pressed_key == Key.down):
            print('Driving back')
            sock.send("b:100\r")
        elif (pressed_key == Key.left):
            print('Counter-clock')
            sock.send("c:900\r")
        elif (pressed_key == Key.right):
            print('Clock')
            sock.send("w:900\r")
        else:
            print('Wrong key')
            return

def on_release(key):
    global pressed_key
    pressed_key = None
    if key == Key.esc:
        # Stop listener
        sock.close()
        return False
    else:
        sock.send("s:0\r")
        return

# Collect events until released
with Listener(
        on_press=on_press,
        on_release=on_release) as listener:
    listener.join()