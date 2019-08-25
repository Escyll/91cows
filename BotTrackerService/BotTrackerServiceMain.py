import argparse
import CameraCalibration as cal
import ArucoTracker as tracker

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--calibrate", action='store_true', default=False, help='Perform camera calibration')
    ap.add_argument("--visualize", action='store_true', default=False, help='Visualize the tracking on a preview window')
    args = ap.parse_args()

    cal_obj = cal.CameraCalibration(use_precaptured_images = not args.calibrate)
    if args.calibrate:
        cal_obj.calibrate_camera()
    camera_matrix, dist_matrix = cal_obj.extract_existing_calibration_data()

    tracker_obj = tracker.ArucoTracker(camera_matrix, dist_matrix, visualization=args.visualize)
    tracker_obj.track_aruco_markers()
    
if __name__ == '__main__':
    main()