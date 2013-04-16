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

void test_interested(int sockfd) {
    bool success = send_interested(sockfd);
    assert(success);
}

void test_request(int sockfd) {
    bool success = send_request(sockfd, 0x0001, 0x0000);
    assert(success);
}

void test_piece(int sockfd) {
    string data = "foobar";
    bool success = send_piece(sockfd, 0x0005, 0x0000, data.c_str(), data.size());
    assert(success);
}

int main() {
    string dst_ip("127.0.0.1");
    int dst_port = 9999;
    int sockfd = init_outgoing_connection(dst_ip, dst_port);
    assert(sockfd > 0);

    test_choke(sockfd);
    test_interested(sockfd);
    test_request(sockfd);
    test_piece(sockfd);
}
