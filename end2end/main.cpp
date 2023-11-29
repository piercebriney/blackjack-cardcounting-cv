#include <chrono>
#include "opencv2/opencv.hpp"
#include "yolov8.hpp"
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <algorithm>
#include <array>
#include <vector>
#include <errno.h>
#include <unordered_set>

using std::chrono::system_clock;
using std::chrono::time_point;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::duration_cast;

// constexpr int NUM_LABELS = 11;
constexpr int NUM_LABELS = 52;
constexpr int FRAME_THRESHOLD = 5;
constexpr int FRAME_MAX = 10;
constexpr float MIN_CONFIDENCE = 0.50;

struct Ticks {
    std::array<int, NUM_LABELS> ticks{};
    std::array<bool, NUM_LABELS> seen{};
    void dec() {
        for (int i = 0; i < NUM_LABELS; ++i) {
            ticks[i] = std::max(ticks[i] - 1, 0);
        }
    }
    void inc(int idx) {
        ticks[idx] = std::min(ticks[idx] + 2, FRAME_MAX);
    }

    template<typename F>
    void extract(F&& f) {
        for (int i = 0; i < NUM_LABELS; ++i) {
            if (ticks[i] > FRAME_THRESHOLD && !seen[i]) {
                f(i);
                seen[i] = true;
            }
        }
    }
    void clear() {
        ticks.fill(0);
        seen.fill(false);
    }
    void debug() {
        for (int i = 0; i < NUM_LABELS; ++i) {
            std::cout << i << " " << ticks[i] << std::endl;
        }
    }
};

struct Timer {
    time_point<system_clock> start;
    void reset() {
        start = system_clock::now();
    }
    milliseconds elapsed() {
        auto end = system_clock::now();
        return duration_cast<milliseconds>(end - start);
    }
    int elapsed_ms() {
        return elapsed().count();
    }
};

struct FrameProcessor {
    Ticks player_ticks{};
    Ticks dealer_ticks{};
    cv::Size size;
    FrameProcessor(cv::Size& s) : size{s} {}

    void process(std::vector<Object>& objs) {
        std::array<Object, NUM_LABELS> best{};
        dealer_ticks.dec();
        player_ticks.dec();
        // deduplicate cards, keeping most confident result (2 boxes for 1 card if the model looks at the corners)
        for (auto& o: objs) {
            if (o.prob >= MIN_CONFIDENCE && o.prob > best[o.label].prob) {
                best[o.label] = o;
            }
        }
        // increment ticks of detected cards
        for (auto& o: best) {
            if (o.prob != 0) {
                if (in_top_half(o.rect, size.height)) {
                    dealer_ticks.inc(o.label);
                } else {
                    player_ticks.inc(o.label);
                }
            }
        }
    }
    void clear() {
        player_ticks.clear();
        dealer_ticks.clear();
    }
};

struct Bridge {
    int fd;
    Bridge(char* ip, unsigned short port) {
        fd = socket(AF_INET, SOCK_STREAM, 0);
        ASSERT(fd >= 0);
        int one = 1;
        ASSERT(setsockopt(fd, SOL_TCP, TCP_NODELAY, (void*)&one, sizeof(int)) == 0);
        struct sockaddr_in s;
        s.sin_family = AF_INET;
        s.sin_addr.s_addr = inet_addr(ip);
        s.sin_port = htons(port);
        ASSERT(connect(fd, (struct sockaddr*)&s, sizeof(s)) == 0);
    }
    void send_multiplier(int multiplier) {
        int sent = send(fd, (char*)&multiplier, sizeof(int), 0);
        ASSERT(sent == sizeof(int));
    }
    bool should_reset() {
        char dummy[1];
        int read = recv(fd, &dummy, 1, MSG_DONTWAIT);
        bool ret = false;
        if (read < 0) {
            ASSERT(errno == EAGAIN || errno == EWOULDBLOCK);
        } else {
            ret = read == 1;
        }
        return ret;
    }
    ~Bridge() {
        close(fd);
    }
};

// const std::vector<std::string> CLASS_NAMES = {"ace", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten", "face"};
const std::vector<std::string> CLASS_NAMES = {"10C", "10D", "10H", "10S", "2C", "2D", "2H", "2S", "3C", "3D", "3H", "3S", "4C", "4D", "4H", "4S", "5C", "5D", "5H", "5S", "6C", "6D", "6H", "6S", "7C", "7D", "7H", "7S", "8C", "8D", "8H", "8S", "9C", "9D", "9H", "9S", "AC", "AD", "AH", "AS", "JC", "JD", "JH", "JS", "KC", "KD", "KH", "KS", "QC", "QD", "QH", "QS"};

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
    std::cout << "Bridge connected!\n";

    // cv::Size size{1280, 1280};
    cv::Size size{640, 640};
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

    while (true) {
        t.reset();
        if (!cap.read(image)) {
            break;
        }
        auto read_ms = t.elapsed_ms();
        t.reset();
        yolov8.run(image, objs);

        fp.process(objs);

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
            std::cout << "round over\n";
            // TODO: compute multiplier from game state
            int n = 42;
            bridge.send_multiplier(n);
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
