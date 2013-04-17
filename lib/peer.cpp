#include "peer.h"
#include "bencode.h"

using namespace std;

/*=== Peer ===*/
Peer::Peer() : peer_id(), ip(), port() {}

Peer::Peer(string pid, string ip, int port) :
    peer_id(pid), ip(ip), port(port),
    client_choking(false), client_interested(false),
    peer_interested(false), peer_choking(false),
    handshaked(false) {}

Peer Peer::parse(map<std::string, BencodeTokenPtr> peer_dict) {
    // extracts information from the peer dictionary in the tracker response
    // creates a Peer object from this information
    string peer_id = BencodeString::get_value(peer_dict["peer id"]);
    string ip = BencodeString::get_value(peer_dict["ip"]);
    int port = BencodeInteger::get_value(peer_dict["port"]);
    return Peer(peer_id, ip, port);
}

/*=== PeerManager ===*/
PeerManager::PeerManager() : mConnectedPeers() {}

void PeerManager::add_peer(int conn, std::string peer_id, std::string ip, int port) {
    if(mConnectedPeers.find(conn) == mConnectedPeers.end()) {
        mConnectedPeers[conn] = Peer(peer_id, ip, port);
    }
}

// handlers for messages
void PeerManager::handle_choke(int conn) {
    if(mConnectedPeers.find(conn) == mConnectedPeers.end()) {
        return;
    }
    mConnectedPeers[conn].peer_choking = true;
}

void PeerManager::handle_unchoke(int conn) {
    if(mConnectedPeers.find(conn) == mConnectedPeers.end()) {
        return;
    }
    mConnectedPeers[conn].peer_choking = false;
    //mActivePeers.push_back(peer_id);
}


void PeerManager::handle_interested(int conn) {
    if(mConnectedPeers.find(conn) == mConnectedPeers.end()) {
        return;
    }
    mConnectedPeers[conn].peer_interested = true;
}

void PeerManager::handle_uninterested(int conn) {
    if(mConnectedPeers.find(conn) == mConnectedPeers.end()) {
        return;
    }
    mConnectedPeers[conn].peer_interested = false;
}
