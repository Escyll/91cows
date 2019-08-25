import cv2
import numpy as np
import os
import GlobalConstants as gc
import sys


class ArucoTracker(object):
    def __init__(self, camera_matrix, dist_matrix, visualization=True):
        self.__camera_matrix = camera_matrix
        self.__dist_matrix = dist_matrix
        self.__visualization = visualization

    def __initialize_capture(self):
        self.__cap = cv2.VideoCapture(gc.g_source)
        if self.__visualization:
            self.__preview_window = cv2.namedWindow(
                gc.g_tracker_live_window, cv2.WINDOW_AUTOSIZE)

    def __stop_aruco_tracking(self):
        print("Exiting Application")
        self.__cap.release()
        if self.__visualization:
            cv2.destroyWindow(gc.g_tracker_live_window)

    def __del__(self):
        self.__stop_aruco_tracking()

    def __read_frame(self):
        ret, frame = self.__cap.read()
        if not ret:
            sys.exit("no data in incoming frame")
        return frame

    def __initialize_aruco_dict_and_params(self):
        self.__aruco_dict = cv2.aruco.Dictionary_get(cv2.aruco.DICT_4X4_50)
        self.__parameters = cv2.aruco.DetectorParameters_create()
        self.__parameters.adaptiveThreshConstant = 10
        self.__font = cv2.FONT_HERSHEY_SIMPLEX

    def track_aruco_markers(self):
        print("Running aruco marker tracker")
        self.__initialize_capture()
        self.__initialize_aruco_dict_and_params()
        while(True):
            frame = self.__read_frame()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            corners, ids, _ = cv2.aruco.detectMarkers(
                gray, self.__aruco_dict, parameters=self.__parameters)
            if np.all(ids != None):
                rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(
                    corners, gc.g_marker_length_in_meters, self.__camera_matrix, self.__dist_matrix)

                for i in range(0, ids.size):
                    cv2.aruco.drawAxis(frame, self.__camera_matrix, self.__dist_matrix,
                                       rvec[i], tvec[i], gc.g_marker_length_in_meters)

                if self.__visualization:
                    cv2.aruco.drawDetectedMarkers(frame, corners)
                    strg = ''
                    for i in range(0, ids.size):
                        strg += str(ids[i][0])+', '
                    cv2.putText(frame, "Id: " + strg, (0, 64),
                                self.__font, 1, (0, 255, 0), 2, cv2.LINE_AA)
            else:
                if self.__visualization:
                    cv2.putText(frame, "No Ids", (0, 64),
                                self.__font, 1, (0, 255, 0), 2, cv2.LINE_AA)

            if self.__visualization:
                cv2.imshow(gc.g_tracker_live_window, frame)

            if cv2.waitKey(int(1000/gc.g_fps)) & 0xFF == ord('q'):
                self.__stop_aruco_tracking()
                break
