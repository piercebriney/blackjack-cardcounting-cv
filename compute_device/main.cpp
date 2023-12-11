#include "opencv2/opencv.hpp"
#include "yolov8.hpp"
#include <vector>
#include "game.h"
#include "fp.h"
#include "bridge.h"
#include "timer.h"

int main(int argc, char** argv) {
    cudaSetDevice(0);

    if (argc != 5) {
        puts("Usage: compute_device <ENGINE PATH> <VIDEO URL> <PI IP> <GPIO PORT>");
        return 1;
    }
    Timer t;
    char* bridge_ip = argv[3];
    unsigned short bridge_port = std::stoi(argv[4]);

    const std::string engine_file_path{argv[1]};
    const std::string video_source{argv[2]};

    Bridge bridge{bridge_ip, bridge_port};

    YOLOv8 yolov8{engine_file_path, SIZE};
    yolov8.make_pipe(true);

    cv::Mat image;
    std::vector<Object> objs;

    FrameProcessor fp;

    cv::namedWindow("Result", cv::WINDOW_AUTOSIZE);
    cv::VideoCapture cap{video_source};
    ASSERT(cap.isOpened());

    Game g;
    int seen_nothing_ms = 0;

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

        // printf("read %d process %d\n", read_ms, process_ms);
        // printf("seen nothing for %d ms\n", seen_nothing_ms);
        if (seen_nothing_ms > 1000 && g.is_actionable_loose()) {
            puts("CLEAR");
            seen_nothing_ms = 0;
            int bet_value = g.p.getBet();
            printf("BET $%d\n", bet_value);
            bridge.send_multiplier(bet_value > G_MAXIMUM_BET / 2);
            fp.clear();
            g.reset_round();
        }

        if (bridge.should_reset()) {
            puts("FULL RESET");
            fp.clear();
            g.reset_shoe();
        }

        if (cv::waitKey(10) == 'q') {
            break;
        }
    }

    cv::destroyAllWindows();
    return 0;
}
