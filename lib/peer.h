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
    static Peer parse(map<std::string, BencodeTokenPtr> peer_dict);
    string peer_id;
    // peer's IP address either IPv6 (hexed) or IPv4 (dotted quad) or DNS name (string)
    string ip;
    int port;
};

// Manages known peers in the swarm
class PeerManager {
public:
    std::vector<Peer> peer_list;
    std::vector<Peer> active_peers;
};

#endif
