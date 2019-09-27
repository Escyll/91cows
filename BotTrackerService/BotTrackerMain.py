import argparse
import CameraCalibration as cal
import ArucoTracker as tracker
import Communicator as com

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--calibrate", action='store_true', default=False, help='Perform camera calibration')
    ap.add_argument("--visualize", action='store_true', default=False, help='Visualize the tracking on a preview window')
    ap.add_argument("--communicate", action='store_true', default=False, help='Transmit data via tcp')
    ap.add_argument("--record", action='store_true', default=False, help="Record robot coordinates in a file for use by simulator")
    args = ap.parse_args()

    cal_obj = cal.CameraCalibration(use_precaptured_images = not args.calibrate)
    if args.calibrate:
        cal_obj.calibrate_camera()
    camera_matrix, dist_matrix = cal_obj.extract_existing_calibration_data()

    com_obj = None
    if args.communicate:
        com_obj = com.Communicator(args.record)

    tracker_obj = tracker.ArucoTracker(camera_matrix, dist_matrix, com_obj, visualization=args.visualize)
    tracker_obj.track_aruco_markers()
    
if __name__ == '__main__':
    main()