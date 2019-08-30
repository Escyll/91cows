import numpy as np
import cv2
import glob
import sys
import os
import GlobalConstants as gc


class CameraCalibration(object):
    def __init__(self, use_precaptured_images):
        self.__use_precaptured_images = use_precaptured_images
        self.__chessboard_dim = (9, 6)
        self.__saved_calib_images = []

    def __initialize_capture(self):
        if not self.__use_precaptured_images:
            self.__cap = cv2.VideoCapture(gc.g_source)
            self.__cap.set(cv2.CAP_PROP_FRAME_WIDTH, gc.g_frame_width)
            self.__cap.set(cv2.CAP_PROP_FRAME_HEIGHT, gc.g_frame_height)
            self.__preview_window = cv2.namedWindow(
                gc.g_calibration_preview_window, cv2.WINDOW_AUTOSIZE)

    def __del__(self):
        self.__exit_calibration()

    def __read_frame(self):
        ret, frame = self.__cap.read()
        if not ret:
            sys.exit("no data in incoming frame")
        return frame

    def __save_calibration_images(self, frame):
        self.__saved_calib_images.append(frame)
        print("Saving frame to calibration images : {0}/ (minimum required {1})".format(
            len(self.__saved_calib_images), gc.g_minimum_calib_images))

    def __perform_calibration_from_saved_images(self):
        if len(self.__saved_calib_images) < gc.g_minimum_calib_images:
            print("Insufficient calibration data, expected {0}, found {1}".format(
                gc.g_minimum_calib_images, len(self.__saved_calib_images)))
            print("Acquire {0} more images and restart calibration!".format(
                gc.g_minimum_calib_images - len(self.__saved_calib_images)))
            return

        # termination criteria
        termination_max_epsilon = 0.001
        termination_max_iter = 30
        criteria = (cv2.TERM_CRITERIA_EPS + cv2.TERM_CRITERIA_MAX_ITER,
                    termination_max_iter, termination_max_epsilon)
        # prepare object points, like (0,0,0), (1,0,0), (2,0,0) ....,(6,5,0)
        objp = np.zeros(
            (self.__chessboard_dim[1] * self.__chessboard_dim[0], 3), np.float32)
        objp[:, :2] = np.mgrid[0:self.__chessboard_dim[0],
                               0:self.__chessboard_dim[1]].T.reshape(-1, 2)
        obj_points = []  # 3d points in real world space
        img_points = []  # 2d points in image plane

        print("Starting calibration from saved images")
        self.__progress_window = cv2.namedWindow(
            gc.g_calibration_progress_window, cv2.WINDOW_AUTOSIZE)
        for frame in self.__saved_calib_images:
            found, corners, gray = self.__find_and_draw_chessboard_corners(
                frame, visualize=False)
            if found:
                obj_points.append(objp)
                refined_corners = cv2.cornerSubPix(
                    gray, corners, (11, 11), (-1, -1), criteria)
                img_points.append(refined_corners)

                # Draw and display the refined corners
                frame = cv2.drawChessboardCorners(
                    frame, self.__chessboard_dim, refined_corners, found)
                cv2.imshow(gc.g_calibration_progress_window, frame)
                cv2.waitKey(int(1000/gc.g_fps))

        cv2.destroyWindow(gc.g_calibration_progress_window)
        self.__calibrate_camera_and_save_yaml(
            obj_points, img_points, gray.shape[::-1])

    def __calibrate_camera_and_save_yaml(self, obj_points, img_points, frame_dims):
        print("Creating calibration data for the camera")
        _, mtx, dist, _, _ = cv2.calibrateCamera(
            obj_points, img_points, frame_dims, None, None)

        print("Saving calibration to disk . . .")
        if not os.path.exists(gc.g_calib_data_folder):
            os.makedirs(gc.g_calib_data_folder)
        file_path = os.path.join(
            gc.g_calib_data_folder, gc.g_calib_data_file_name)
        cv_file = cv2.FileStorage(file_path, cv2.FILE_STORAGE_WRITE)
        cv_file.write("camera_matrix", mtx)
        cv_file.write("dist_coeff", dist)
        cv_file.release()
        print("Saved calibration to disk {0}".format(file_path))

    def __find_and_draw_chessboard_corners(self, frame, visualize=True):
        if visualize:
            draw_to_frame = frame.copy()

        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        found, corners = cv2.findChessboardCorners(
            frame, self.__chessboard_dim, None)

        if visualize:
            cv2.drawChessboardCorners(
                draw_to_frame, self.__chessboard_dim, corners, found)
            cv2.imshow(gc.g_calibration_preview_window, draw_to_frame)
        return found, corners, gray

    def __dump_saved_images_to_disk(self):
        if len(self.__saved_calib_images) < gc.g_minimum_calib_images:
            print("Insufficient calibration data, expected {0}, found {1}".format(
                gc.g_minimum_calib_images, len(self.__saved_calib_images)))
            print("Acquire {0} more images and attempt dump to disk!".format(
                gc.g_minimum_calib_images - len(self.__saved_calib_images)))
            return

        if not os.path.exists(gc.g_calib_images_folder):
            os.makedirs(gc.g_calib_images_folder)
        else:
            images = glob.glob(os.path.join(gc.g_calib_images_folder, "*.jpg"))
            if len(images):
                print("Existing images found . . . Deleting . . .")
                for fname in images:
                    try:
                        if os.path.isfile(fname):
                            os.unlink(fname)
                    except Exception as e:
                        print(e)

        # save images to disk
        print("Saving images to disk {0}".format(gc.g_calib_images_folder))
        for img_idx in range(0, len(self.__saved_calib_images)):
            fname = "calibration_image_" + str(img_idx) + ".jpg"
            cv2.imwrite(os.path.join(gc.g_calib_images_folder, fname),
                        self.__saved_calib_images[img_idx])
        print("Saved {0} images to disk".format(
            len(self.__saved_calib_images)))

    def __read_images_from_disk_and_calibrate(self):
        if not os.path.exists(gc.g_calib_images_folder):
            sys.exit("No saved images found")

        images = glob.glob(os.path.join(gc.g_calib_images_folder, "*.jpg"))
        if len(images) < gc.g_minimum_calib_images:
            sys.exit("Insufficient calibration imgages on disk: expected atleast {0}, but found {1}".format(
                gc.g_minimum_calib_images, len(images)))

        for image_file in images:
            image = cv2.imread(image_file)
            self.__saved_calib_images.append(image)

        print("Successfully read {0} images from disk".format(
            len(self.__saved_calib_images)))
        self.__perform_calibration_from_saved_images()

    def __load_cal_data_from_file(self, cal_data_file):
        print("Retrieving existing calibration from disk")
        cv_file = cv2.FileStorage(cal_data_file, cv2.FILE_STORAGE_READ)
        camera_matrix = cv_file.getNode("camera_matrix").mat()
        dist_matrix = cv_file.getNode("dist_coeff").mat()
        return camera_matrix, dist_matrix

    def __exit_calibration(self):
        print("Exiting calibration")
        if not self.__use_precaptured_images:
            self.__cap.release()
            cv2.destroyWindow(gc.g_calibration_preview_window)
            cv2.destroyWindow(gc.g_calibration_progress_window)

    def calibrate_camera(self):
        if self.__use_precaptured_images:
            self.__read_images_from_disk_and_calibrate()
        else:
            self.__initialize_capture()
            print(
                "space to save image, c to start calibration, d to dump images to disk, q to quit")
            while(True):
                frame = self.__read_frame()
                self.__find_and_draw_chessboard_corners(frame)

                character = cv2.waitKey(int(1000/gc.g_fps))
                if character == ord(' '):
                    self.__save_calibration_images(frame)
                elif character == ord('c'):
                    self.__perform_calibration_from_saved_images()
                elif character == ord('d'):
                    self.__dump_saved_images_to_disk()
                elif character == ord('q'):
                    self.__exit_calibration()
                    break
                else:
                    continue

    def extract_existing_calibration_data(self, cal_data_file=os.path.join(gc.g_calib_data_folder, gc.g_calib_data_file_name)):
        if not os.path.isfile(cal_data_file):
            sys.exit(
                "Expected calibration file does not exist: {0}".format(cal_data_file))

        return self.__load_cal_data_from_file(cal_data_file)


# Testing purposes
if __name__ == "__main__":
    calObj = CameraCalibration(use_precaptured_images=True)
    calObj.calibrate_camera()
    calObj.extract_existing_calibration_data()
