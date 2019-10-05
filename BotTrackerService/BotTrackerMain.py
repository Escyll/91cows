import argparse
import CameraCalibration as cal
import ArucoTracker as tracker
import Communicator as com
import FieldSetter as fs
import DataRecorder as rec


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--calibrate", action='store_true',
                    default=False, help='Perform camera calibration')
    ap.add_argument("--visualize", action='store_true', default=False,
                    help='Visualize the tracking on a preview window')
    ap.add_argument("--communicate", action='store_true',
                    default=False, help='Transmit data via tcp')
    ap.add_argument("--record", action='store_true', default=False,
                    help="Record robot coordinates in a file for use by sim")
    args = ap.parse_args()

    cal_obj = cal.CameraCalibration(use_precaptured_images=not args.calibrate)
    if args.calibrate:
        cal_obj.calibrate_camera()
    camera_matrix, dist_matrix = cal_obj.extract_existing_calibration_data()
    del(cal_obj)

    field_set_obj = fs.FieldSetter(visualization=True)
    ref_point = field_set_obj.select_playing_field()
    del(field_set_obj)

    com_obj = None
    if args.communicate:
        com_obj = com.Communicator()

    rec_obj = None
    if args.record:
        rec_obj = rec.DataRecorder()

    tracker_obj = tracker.ArucoTracker(camera_matrix, dist_matrix, com_obj,
                                       rec_obj, ref_point,
                                       visualization=args.visualize)
    tracker_obj.track_aruco_markers()


if __name__ == '__main__':
    main()
