cc_binary(
    name = "feature_extractor_single_image",
    srcs = ["feature_extractor_single_image.cc"],
    deps = [
        "//src/features/eye:eye_analyzer",
        "//src/features/mouth:mouth_analyzer",
        "//src/features/face:face_analyzer",
        "//mediapipe/modules/face_landmark:face_landmark_front_cpu",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:commandlineflags",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
    ],
)

cc_binary(
    name = "feature_extractor_video",
    srcs = ["feature_extractor_video.cc"],
    deps = [
        "//src/features/eye:eye_analyzer",
        "//src/features/mouth:mouth_analyzer",
        "//src/features/face:face_analyzer",
        "//src/calculators:video_reader_calculator",
        "//src/calculators:landmarks_to_features_calculator",
        "//src/calculators:array_to_csv_row_calculator",
        "//mediapipe/modules/face_landmark:face_landmark_front_cpu",
        "//mediapipe/framework:calculator_framework",
        "//mediapipe/framework/formats:image_frame",
        "//mediapipe/framework/formats:image_frame_opencv",
        "//mediapipe/framework/port:commandlineflags",
        "//mediapipe/framework/port:file_helpers",
        "//mediapipe/framework/port:opencv_highgui",
        "//mediapipe/framework/port:opencv_imgproc",
        "//mediapipe/framework/port:opencv_video",
        "//mediapipe/framework/port:parse_text_proto",
        "//mediapipe/framework/port:status",
    ],
)