#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <errno.h>
#include "common.hpp"

#pragma once
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
    void send_action(action a) {
        char msg[2];
        msg[0] = a;
        msg[1] = 0;
        int sent = send(fd, &msg, sizeof(msg), 0);
        ASSERT(sent == sizeof(msg));
    }
    void send_multiplier(int multiplier) {
        char msg[2];
        msg[0] = 0x69;
        // assume less than 256
        msg[1] = multiplier;
        int sent = send(fd, &msg, sizeof(msg), 0);
        ASSERT(sent == sizeof(msg));
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

