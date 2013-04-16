#include "peer.h"
#include "bencode.h"

using namespace std;

/*=== Peer ===*/
Peer::Peer(string pid, string ip, int port) :
    peer_id(pid), ip(ip), port(port),
    client_choking(false), client_interested(false), 
    peer_interested(false), peer_choking(false) {
    pieces = NULL;
}

Peer Peer::parse(map<std::string, BencodeTokenPtr> peer_dict) {
    // extracts information from the peer dictionary in the tracker response
    // creates a Peer object from this information
    string peer_id = BencodeString::get_value(peer_dict["peer id"]);
    string ip = BencodeString::get_value(peer_dict["ip"]);
    int port = BencodeInteger::get_value(peer_dict["port"]);
    return Peer(peer_id, ip, port);
}

void Peer::init_piece_field(int num_pieces) {
    this->pieces(num_pieces);
    this->num_pieces = num_pieces;
}


/*=== PeerManager ===*/
PeerManager::PeerManager(vector<Peer> peer_list) : mPeers(), mActivePeers() {
    mPeers();
    // todo init bitfields
    for(int i=0; i<peer_list.size(); i++) {
        mPeers[peer_list[i].peer_id] = peer_list[i];
    }
}


void PeerManager::handle_handshake(std::string peer_id, std::string ip, int port) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        mPeers[peer_id] = Peer(peer_id, ip, port);
    }
}

// handlers for messages
void PeerManager::handle_choke(std::string peer_id) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        return;
    }
    mPeers[peer_id].peer_choking = true;
}

void PeerManager::handle_unchoke(std::string peer_id) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        return;
    }
    mPeers[peer_id].peer_choking = false;
}


void PeerManager::handle_interested(std::string peer_id) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        return;
    }
    mPeers[peer_id].peer_interested = true;
}

void PeerManager::handle_uninterested(std::string peer_id) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        return;
    }
    mPeers[peer_id].peer_interested = false;
}

void PeerManager::handle_have(std::string peer_id, int piece_id) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        return;
    }
    if(mPeers[peer_id].pieces == NULL) {
        return;
    }
    mPeers[peer_id].pieces[piece_id] = false;
}

void PeerManager::handle_bitfield(std::string peer_id, unsigned long bitfield, size_t n) {
    if(mPeers.find(peer_id) == mPeers.end()) {
        return;
    }
    if(mPeers[peer_id].pieces == NULL) {
        return;
    }
    
    Peer& peer = mPeers[peer_id];

    // TODO: move num_pieces to piece manager or btsession
    peer.pieces(peer.num_pieces, bitfield);
    for(int i=0; i<n; i++) {
    }
}
