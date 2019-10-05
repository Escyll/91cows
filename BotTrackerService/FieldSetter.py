import cv2
import numpy as np
import os
import GlobalConstants as gc
import sys


class FieldSetter(object):
    def __init__(self, visualization=True):
        self.__visualization = visualization
        self.__ref_point = [(0, 0), (0, 0)]

    def __initialize_capture(self):
        self.__cap = cv2.VideoCapture(gc.g_source)
        self.__cap.set(cv2.CAP_PROP_FRAME_WIDTH, gc.g_frame_width)
        self.__cap.set(cv2.CAP_PROP_FRAME_HEIGHT, gc.g_frame_height)
        if self.__visualization:
            self.__preview_window = cv2.namedWindow(
                gc.g_field_selection_live_window, cv2.WINDOW_AUTOSIZE)
        cv2.setMouseCallback(
            gc.g_field_selection_live_window, self.__shape_selection)

    def __stop_field_selection(self):
        print("Exiting Field Selection")
        self.__cap.release()
        if self.__visualization:
            cv2.destroyWindow(gc.g_field_selection_live_window)

    def __del__(self):
        self.__stop_field_selection()

    def __read_frame(self):
        ret, frame = self.__cap.read()
        if not ret:
            sys.exit("no data in incoming frame")
        return frame

    def __shape_selection(self, event, x, y, flags, param):
        if event == cv2.EVENT_LBUTTONDOWN:
            self.__ref_point[0] = (x, y)
        elif event - - cv2.EVENT_LBUTTONUP:
            self.__ref_point[1] = (x, y)

    def __draw_selection(self, frame):
        if self.__visualization:
            cv2.rectangle(
                frame, self.__ref_point[0], self.__ref_point[1], (0, 255, 0))
            cv2.imshow(gc.g_field_selection_live_window, frame)

    def select_playing_field(self):
        print("Running playing field selection utility")
        print("Draw rectangle over the playing field and press d when done")
        self.__initialize_capture()
        while(True):
            frame = self.__read_frame()
            key = cv2.waitKey(int(1000/gc.g_fps)) & 0xFF
            if key == ord('d'):
                break
            self.__draw_selection(frame)

        self.__stop_field_selection()
        return self.__ref_point
