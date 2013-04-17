#ifndef BTSESSION_H
#define BTSESSION_H

#include "common.h"
#include "piece.h"
#include "peer.h"
#include "tracker.h"
#include "metainfo.h"
#include "peerwire.h"

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

    void connect_peer(Peer p);
    void listen_incoming_msg();
    void handle_msg(int sockfd, uint8_t *msg, size_t n, std::string src_ip, int src_port);

    MetaInfo mMetainfo;
    TrackerConnection mTrackerConnection;
    PeerManager mPeerManager;
    PieceManager mPieceManager;

    int listen_conn;
    std::vector<int> open_connections;
};
#endif
