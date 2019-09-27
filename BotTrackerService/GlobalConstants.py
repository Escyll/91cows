import numpy as np

g_fps = 20
g_source = 0
g_calibration_preview_window = "Camera Calibration Preview Window"
g_calibration_progress_window = "Camera Calibration Progress Window"
g_tracker_live_window = "Live Aruco Tracker"
g_minimum_calib_images = 20
g_calib_images_folder = "calib_images"
g_calib_data_folder = "calib_data"
g_calib_data_file_name = "camera_calibration.yaml"
g_sim_data_file_name = "simulator_data.txt"
g_marker_length_in_meters = 0.1
g_visible_axes_length = 0.05
# g_frame_width = 1920
# g_frame_height = 1080
g_frame_width = 1280
g_frame_height = 720
g_axis = np.float32([[0, 0, 0], [g_visible_axes_length, 0, 0],
                     [0, g_visible_axes_length, 0], [0, 0, g_visible_axes_length]])
g_empty_com_data = []
g_server_address = ('localhost', 10000)
g_end_marker = '\n'