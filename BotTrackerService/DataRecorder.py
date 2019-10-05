import sys
import json
import GlobalConstants as gc


class DataRecorder(object):
    def __init__(self, ):
        print("Recording simulator data in file {}".format(
            gc.g_sim_data_file_name))
        self.__data_file = open(gc.g_sim_data_file_name, "w")

    def __del__(self):
        print("Closing simulator data file")
        self.__data_file.close()

    def __record_data(self, data):
        self.__data_file.write(data)

    def record_data(self, data):
        if data:
            json_data = json.dumps(data) + gc.g_end_marker
            self.__record_data(json_data)
