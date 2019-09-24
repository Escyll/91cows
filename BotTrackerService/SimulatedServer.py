import socket
import sys
import GlobalConstants as gc

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port
print('starting up on {} port {}'.format(*gc.g_server_address))
sock.bind(gc.g_server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection')
    connection, client_address = sock.accept()
    try:
        print('connection from', client_address)
        while True:
            total_data = ''
            while True:
                data = connection.recv(1).decode('utf-8')
                total_data += data
                if data == gc.g_end_marker:
                    break
            print('received {!r}'.format(total_data))
            
            if data:
                print('data received from', client_address)
            else:
                print('no data from', client_address)
                break

    finally:
        # Clean up the connection
        connection.close()