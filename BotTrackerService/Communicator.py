import socket
import sys
import json
import GlobalConstants as gc

class Communicator(object):
    def __init__(self, record_data = False, server_address = gc.g_server_address):
        print("Initializing communicator")
        # Create a TCP/IP socket
        self.__sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        print("Connecting to {} port {}".format(*server_address))
        self.__sock.connect(server_address)
        self.__record = record_data
        if self.__record:
            print("Recording simulator data in file {}".format(gc.g_sim_data_file_name))
            self.__data_file = open(gc.g_sim_data_file_name, "w")

    def __del__(self):
        print("Closing socket")
        self.__sock.close()
        if self.__record:
            print("Closing simulator data file")
            self.__data_file.close()

    def __record_data(self, data):
        if self.__record:
            self.__data_file.write(data)

    def transmit_data(self, data):
        if data:
            json_data = json.dumps(data) + gc.g_end_marker
            self.__record_data(json_data)
            self.__sock.sendall(json_data.encode('utf-8'))
