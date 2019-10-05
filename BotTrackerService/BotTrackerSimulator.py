import argparse
import os
import sys
import json
import time
import Communicator as com
import GlobalConstants as gc


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--server", type=str, default='localhost',
                    help='server address (default localhost)')
    ap.add_argument("--port", type=int, default=10000,
                    help='server port (default 10000)')
    ap.add_argument("--fps", type=int, default=20,
                    help='data rate in fps for simulator (default 20)')
    args = ap.parse_args()

    com_obj = com.Communicator(server_address=(args.server, args.port))
    if not os.path.exists(gc.g_sim_data_file_name):
        sys.exit("No recorded simulator data found. Quitting . . .")

    data_file = open(gc.g_sim_data_file_name, "r")
    data_contents = data_file.readlines()
    data_file.close()

    print("Transmitting simulator data")
    for line in data_contents:
        json_data = json.loads(line)
        time.sleep((1/args.fps))
        com_obj.transmit_data(json_data)


if __name__ == '__main__':
    main()
