#ifndef PEER_H
#define PEER_H

#include <string>
#include <vector>
#include <map>
#include <cstdlib>

#include <boost/shared_ptr.hpp>

#include "common.h"

// Represents a Peer
class Peer {
public:
    static Peer parse(std::map<std::string, BencodeTokenPtr> peer_dict);
    std::string peer_id;
    // peer's IP address either IPv6 (hexed) or IPv4 (dotted quad) or DNS name (string)
    std::string ip;
    int port;
};

// Manages known peers in the swarm
class PeerManager {
public:
    PeerManager(std::vector<Peer> peer_list);
private:
    std::vector<Peer> mPeerList;
    std::vector<Peer> mActivePeers;
};

#endif
