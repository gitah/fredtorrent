#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/dynamic_bitset.hpp>

#include "common.h"

#ifndef PEER_H
#define PEER_H

// Represents a Peer
class Peer {
public:
    Peer();
    Peer(std::string pid, std::string ip, int port);
    static Peer parse(std::map<std::string, BencodeTokenPtr> peer_dict);

    std::string peer_id;

    // peer's IP address either IPv6 (hexed) or IPv4 (dotted quad) or DNS name (string)
    std::string ip;
    int port;

    bool handshaked;

    bool client_choking;
    bool client_interested;
    bool peer_choking;
    bool peer_interested;
};

// Manages known peers in the swarm
class PeerManager {
public:
    PeerManager();

    void add_peer(int conn, std::string peer_id, std::string ip, int port);

    void handle_choke(int conn);
    void handle_unchoke(int conn);

    void handle_interested(int conn);
    void handle_uninterested(int conn);

    std::map<int, Peer> mConnectedPeers;
};

#endif
