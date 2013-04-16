/*Tests sending peerwire messages works correctly*/

// NOTE: tests is not automated due to network calls required
// just open nc on localhost:9999 and check that the messages are sent correctly

#include <cassert>
#include <cstring>
#include <vector>
#include <iostream>
#include "peerwire.h"

using namespace std;

void test_choke(int sockfd) {
    bool success = send_choke(sockfd);
    assert(success);
}

void test_request(int sockfd) {
    bool success = send_request(sockfd, 0x0001, 0x0000);
    assert(success);
}

void test_piece(int sockfd) {
    char *data = "foobar";
    size_t n = 6;
    bool success = send_piece(sockfd, 0x0005, 0x0000, data, n);
    assert(success);
}

int main() {
    string dst_ip("127.0.0.1");
    int dst_port = 9999;
    int sockfd = init_connection(dst_ip, dst_port);
    assert(sockfd != NULL);

    test_choke(sockfd);
}
