import cv2
import numpy as np
import os
import GlobalConstants as gc
import sys


class ArucoTracker(object):
    def __init__(self, camera_matrix, dist_matrix, com_obj, 
                 ref_point=[(0, 0), (gc.g_frame_width, gc.g_frame_height)], visualization=True):
        self.__camera_matrix = camera_matrix
        self.__dist_matrix = dist_matrix
        self.__visualization = visualization
        self.__communicate = False
        self.__ref_point = ref_point
        if com_obj is not None:
            self.__com_obj = com_obj
            self.__com_data = []  # List of dictionaries
            self.__communicate = True

    def __initialize_capture(self):
        self.__cap = cv2.VideoCapture(gc.g_source)
        self.__cap.set(cv2.CAP_PROP_FRAME_WIDTH, gc.g_frame_width)
        self.__cap.set(cv2.CAP_PROP_FRAME_HEIGHT, gc.g_frame_height)
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
        cropped_frame = frame[self.__ref_point[0][1]: self.__ref_point[1][1],
                              self.__ref_point[0][0]: self.__ref_point[1][0]]
        return cropped_frame

    def __transmit_com_data(self):
        if self.__communicate:
            if self.__com_data:
                self.__com_obj.transmit_data(self.__com_data)
            else:
                self.__com_obj.transmit_data(gc.g_empty_com_data)

    def __clear_com_data(self):
        if self.__communicate:
            self.__com_data = []

    def __append_robot_to_com_dict(self, robot_id, imgpts):
        if self.__communicate:
            robot_info = {}
            width = self.__ref_point[1][0] - self.__ref_point[0][0]
            height = self.__ref_point[1][1] - self.__ref_point[0][1]
            center_x = imgpts[0][0][0] / width
            center_y = imgpts[0][0][1] / height
            xorient_x = imgpts[1][0][0] / width
            xorient_y = imgpts[1][0][1] / height
            yorient_x = imgpts[2][0][0] / width
            yorient_y = imgpts[2][0][1] / height
            robot_info["robot"] = {"arucoId": int(robot_id),
                                   "position": [center_x, center_y],
                                   "xorient": [xorient_x - center_x, xorient_y - center_y],
                                   "yorient": [yorient_x - center_x, yorient_y - center_y]}
            self.__com_data.append(robot_info)

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
            self.__clear_com_data()
            frame = self.__read_frame()
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            corners, ids, _ = cv2.aruco.detectMarkers(
                gray, self.__aruco_dict, parameters=self.__parameters)
            if np.all(ids != None):
                rvec, tvec, _ = cv2.aruco.estimatePoseSingleMarkers(
                    corners, gc.g_marker_length_in_meters, self.__camera_matrix, self.__dist_matrix)
                for i in range(0, ids.size):
                    cv2.aruco.drawAxis(frame, self.__camera_matrix, self.__dist_matrix,
                                       rvec[i], tvec[i], gc.g_visible_axes_length)
                    imgpts, _ = cv2.projectPoints(
                        gc.g_axis, rvec[i], tvec[i], self.__camera_matrix, self.__dist_matrix)
                    self.__append_robot_to_com_dict(ids[i][0], imgpts)

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

            self.__transmit_com_data()
            if self.__visualization:
                cv2.imshow(gc.g_tracker_live_window, frame)

            if cv2.waitKey(int(1000/gc.g_fps)) & 0xFF == ord('q'):
                self.__stop_aruco_tracking()
                break
