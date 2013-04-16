/*Tests bencoding function works correctly*/

#include <cassert>
#include <iostream>
#include "metainfo.h"
#include "tracker.h"

#define TORRENT_FILE "tests/test.torrent"

#define TEST_TRACKER_URL "http://torrent.ubuntu.com:6969/"
#define TEST_TRACKER_PORT 6969

using namespace std;

TrackerRequest create_req(MetaInfo info) {
    TrackerRequest req;
    req.peer_id = string(BT_UA) + string("1234");
    req.info_hash = "foobar";
    req.client_port = CLIENT_PORT;
    req.event = "started";
    req.uploaded = 0;
    req.downloaded = 0;
    req.left = 0;

    return req;
}

int main() {
    MetaInfo metainfo(string(TORRENT_FILE));
    TrackerConnection conn = TrackerConnection("http://127.0.0.1", 9010);
    TrackerRequest req = create_req(metainfo); 
    vector<Peer> peers = conn.announce(req);
    assert(peers.size() > 0);
}
