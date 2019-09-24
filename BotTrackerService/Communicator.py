import socket
import sys
import json
import GlobalConstants as gc

class Communicator(object):
    def __init__(self):
        print("Initializing communicator")
        # Create a TCP/IP socket
        self.__sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("Connecting to {} port {}".format(*gc.g_server_address))
        self.__sock.connect(gc.g_server_address)
    
    def __del__(self):
        print("Closing socket")
        self.__sock.close()

    def transmit_data(self, data):
        json_data = json.dumps(data) + gc.g_end_marker
        if data:
            self.__sock.sendall(json_data.encode('utf-8'))
            print('sending {}'.format(json_data))