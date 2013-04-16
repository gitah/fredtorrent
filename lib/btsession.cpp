#include "btsession.h"

using namespace std;

// open metainfo file
// connect to tracker
// get peer list

void BTSession::BTSession(string metainfo_file) {
    mMetainfo = MetaInfo(metainfo_file);
    mTrackerConnection = TrackerConnection(mMetainfo.tracker_url);
    mPeerManager = PeerManager(this->get_peers(mTrackerConnection));
}

/* sends a request to tracker and updates state information based on tracker
 * response 
 *
 * returns true if success otherwise false
 * */
std::vector<Peer> BTSession::get_peers(TrackerConnection *tc) {
    TrackerRequest req = this->createTrackerRequest();
    return tc.announce(req);
}

/* creates a TrackerRequest from members to send to tracker */
TrackerRequest BTSession::create_tracker_request() {
    TrackerRequest req;

    req.peer_id = this->peer_id;
    //req.info_hash = ;   // TODO: url encode

    req.port = CLIENT_PORT;

    //  The total amount of bytes uploaded
    req.uploaded = this->bytes_uploaded;
    req.downloaded = this->bytes_uploaded;
    req.left = this->bytes_left;

    return req;
}
