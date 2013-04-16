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
    Peer(string pid, string ip, int port);
    static Peer parse(std::map<std::string, BencodeTokenPtr> peer_dict);

    // creates the piece array to keep track of which piece peer has
    void init_piece_array(num_pieces);

private:
    std::string peer_id;

    // peer's IP address either IPv6 (hexed) or IPv4 (dotted quad) or DNS name (string)
    std::string ip;
    int port;

    bool client_choking;
    bool client_interested;
    bool peer_choking;
    bool peer_interested;

    // keep track of pieces
    boost::dynamic_bitset pieces;
    int num_pieces;
};

// Manages known peers in the swarm
class PeerManager {
public:
    PeerManager(std::vector<Peer> peer_list);

    // TODO: piece manager ??

    // adds peer to peer manager
    void handle_handshake(std::string peer_id, std::string ip, int port);

    void handle_choke(std::string peer_id);
    void handle_unchoke(std::string peer_id);

    void handle_interested(std::string peer_id);
    void handle_uninterested(std::string peer_id);

    void handle_have(std::string peer_id, int piece_id);
    void handle_bitfield(std::string peer_id, unsigned long bitfield, size_t n);

private:
    std::map<std::string, Peer> mPeers;
    std::vector<Peer> mActivePeers;
};

#endif
