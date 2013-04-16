#ifndef BTSESSION_H
#define BTSESSION_H

#include "common.h"
#include "peer.h"
#include "tracker.h"
#include "metainfo.h"

class BTSession {
public:
    BTSession(std::string metainfo_file);

    bool is_open;
    std::string peer_id; // peer_id, generated at startup
    long listen_port;

    // file transfer stats
    int bytes_uploaded;
    int bytes_downloaded;
    int bytes_left;

private:
    std::vector<Peer> get_peers();
    TrackerRequest create_tracker_request(std::string event_type="");

    MetaInfo mMetainfo;
    TrackerConnection mTrackerConnection;
    PeerManager mPeerManager;

    bool connect_to_tracker
};
#endif
