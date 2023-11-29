#include "opencv2/opencv.hpp"
#include "yolov8.hpp"
#include <vector>
#include "game.h"
#include "fp.h"
#include "bridge.h"
#include "timer.h"

#if 0
constexpr int NUM_LABELS = 52;
const std::vector<std::string> CLASS_NAMES = {"10C", "10D", "10H", "10S", "2C", "2D", "2H", "2S", "3C", "3D", "3H", "3S", "4C", "4D", "4H", "4S", "5C", "5D", "5H", "5S", "6C", "6D", "6H", "6S", "7C", "7D", "7H", "7S", "8C", "8D", "8H", "8S", "9C", "9D", "9H", "9S", "AC", "AD", "AH", "AS", "JC", "JD", "JH", "JS", "KC", "KD", "KH", "KS", "QC", "QD", "QH", "QS"};
cv::Size size{640, 640};
#endif

void test_bridge(Bridge& bridge) {
    std::string line;
    while (true) {
        std::getline(std::cin, line);
        int x = std::stoi(line);
        std::cout << "sending " << x << std::endl;
        bridge.send_multiplier(x);
        std::cout << "reset? " << bridge.should_reset() << std::endl;

    }
}

// tcp://10.230.46.171:8888
int _main(int argc, char** argv) {
    cudaSetDevice(0);

    ASSERT(argc == 5);
    Timer t;
    char* bridge_ip = argv[3];
    unsigned short bridge_port = std::stoi(argv[4]);

    const std::string engine_file_path{argv[1]};
    const std::string video_source{argv[2]};

    Bridge bridge{bridge_ip, bridge_port};

    YOLOv8 yolov8{engine_file_path, size};
    yolov8.make_pipe(true);

    std::cout << "YOLOv8 engine warmed up!\n";

    cv::Mat image;
    std::vector<Object> objs;

    FrameProcessor fp{size};

    int seen_nothing_ms;

    cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap{video_source};
    ASSERT(cap.isOpened());

    std::cout << "Video feed connected!\n";

    Game g;

    while (true) {
        t.reset();
        if (!cap.read(image)) {
            break;
        }
        auto read_ms = t.elapsed_ms();
        t.reset();
        yolov8.run(image, objs);

        fp.process(objs);
        g.process(fp, bridge);

        yolov8.draw_objects(image, objs, CLASS_NAMES);

        cv::imshow("Result", image);
        auto process_ms = t.elapsed_ms();

        if (objs.empty()) {
            seen_nothing_ms += read_ms + process_ms;
        } else {
            seen_nothing_ms = 0;
        }

        if (seen_nothing_ms > 7000) {
            seen_nothing_ms = 0;
            // bridge.send_multiplier(n);
        }

        if (bridge.should_reset()) {
            // full reset
        }

        if (cv::waitKey(10) == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}

int main(int argc, char** argv) {
    _main(argc, argv);
}
