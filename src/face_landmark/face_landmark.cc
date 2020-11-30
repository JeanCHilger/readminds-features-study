// Gets an image and outputs the face landmarks.
//
// Build:
//      bazel build --define MEDIAPIPE_DISABLE_GPU --nocheck_visibility //src/face_landmark:face_landmark
//
// Run:
//      bazel-bin/src/face_landmark/face_landmark --input_image_path=path/to/image.jpg

#include <cstdlib>
#include <iostream>
#include <vector>
#include <cmath>

#include "mediapipe/framework/calculator_framework.h"
#include "mediapipe/framework/formats/image_frame.h"
#include "mediapipe/framework/formats/image_frame_opencv.h"
#include "mediapipe/framework/formats/landmark.pb.h"
#include "mediapipe/framework/port/commandlineflags.h"
#include "mediapipe/framework/port/file_helpers.h"
#include "mediapipe/framework/port/opencv_highgui_inc.h"
#include "mediapipe/framework/port/opencv_imgproc_inc.h"
#include "mediapipe/framework/port/opencv_video_inc.h"
#include "mediapipe/framework/port/parse_text_proto.h"
#include "mediapipe/framework/port/status.h"

DEFINE_string(input_image_path, "",
              "Path to the image.");

//
mediapipe::Status RunGraph() {

    mediapipe::CalculatorGraphConfig config = 
            mediapipe::ParseTextProtoOrDie<mediapipe::CalculatorGraphConfig>(R"(
                input_stream: "IMAGE:input_image"
                output_stream: "LANDMARKS:multi_face_landmarks" 
                node: {
                    calculator: "FaceLandmarkFrontCpu"
                    input_stream: "IMAGE:input_image"
                    output_stream: "LANDMARKS:multi_face_landmarks"
                }
            )");

    // creates the graph with those configs
    mediapipe::CalculatorGraph graph;

    MP_RETURN_IF_ERROR(graph.Initialize(config));

    ASSIGN_OR_RETURN(mediapipe::OutputStreamPoller poller,
                    graph.AddOutputStreamPoller("multi_face_landmarks"));
    
    MP_RETURN_IF_ERROR(graph.StartRun({}));

    // read input image 
    cv::Mat raw_image = cv::imread(FLAGS_input_image_path);

    // cv::resize(raw_image, raw_image, cv::Size(), 0.2, 0.2);

    // wrap cv::Mat into a ImageFrame
    auto input_frame = std::make_unique<mediapipe::ImageFrame>(
        mediapipe::ImageFormat::SRGB,
        raw_image.cols,
        raw_image.rows
    );
    
    cv::Mat input_frame_mat = mediapipe::formats::MatView(input_frame.get());

    cv::cvtColor(raw_image, raw_image, cv::COLOR_BGR2RGB);

    int width = input_frame_mat.size().width;
    int height = input_frame_mat.size().height;

    std::cout << width << std::endl;
    std::cout << height << std::endl;

    raw_image.copyTo(input_frame_mat);

    // send input ImageFrame to graph
    MP_RETURN_IF_ERROR(graph.AddPacketToInputStream(
        "input_image",
        mediapipe::Adopt(input_frame.release())
            .At(mediapipe::Timestamp(0))
    ));

    cv::cvtColor(raw_image, raw_image, cv::COLOR_RGB2BGR);

    // gets graph output
    mediapipe::Packet output_packet;

    
    if (!poller.Next(&output_packet)) return mediapipe::OkStatus();

    // get landmark points
    auto& output_landmark_vector = 
            output_packet.Get<std::vector<mediapipe::NormalizedLandmarkList>>();

    mediapipe::NormalizedLandmarkList face_landmarks = output_landmark_vector[0];
    double x, y;

    for (int j=1; j <= 18; j++){

        cv::Mat landmark_dst;
        raw_image.copyTo(landmark_dst);

        for (int i=(j-1) * 26; i < j * 26 - 1; i++) {
            const mediapipe::NormalizedLandmark landmark = face_landmarks.landmark(i);

            x = (int) floor(landmark.x() * width);
            y = (int) floor(landmark.y() * height);

            cv::circle(landmark_dst, cv::Point(x, y), 10, cv::Scalar(255, 0, 0), -1, 8);
            cv::putText(landmark_dst, std::to_string(i), cv::Point(x, y), cv::FONT_HERSHEY_DUPLEX, 0.6, cv::Scalar(0,0,255));

            cv::imwrite(FLAGS_input_image_path+std::to_string(j)+".jpg", landmark_dst);
            
        }

    }
    
    MP_RETURN_IF_ERROR(graph.CloseInputStream("input_image"));
    return graph.WaitUntilDone();
}


int main(int argc, char** argv) {
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    std::cout << FLAGS_input_image_path << std::endl;

    mediapipe::Status output_status = RunGraph();

    std::cout << output_status.message() << std::endl;


    return EXIT_SUCCESS;
}