#include "piece.h"

using namespace std;
using namespace boost;

PieceManager::PieceManager(int num_pieces, int piece_size) :
    num_pieces(num_pieces), piece_size(piece_size) {
    my_pieces(num_pieces);
    my_pieces.clear();
}

/* updates a piece that we have */
void PieceManager::update_piece(int index, bool have_piece) {
    my_pieces[index] = have_piece;
}

/* updates the pieces that we have based on a bitfield */
void PieceManager::update_pieces(boost::dynamic_bitset bitfield) {
    my_pieces = bitfield;
}

/* returns the client pieces as a bitfield */
unsigned long PieceManager::pieces_to_bitfield() {
    return my_pieces.to_ulong();
}

/* updates a specific piece of a given peer */
void handle_have(std::string peer_id, int piece_id) {
    if(piece_id >= num_pieces) return;

    if(peer_pieces.find(peer_id) == peer_pieces.end()) {
        peer_pieces[peer_id] = dynamic_bitset(num_pieces);
    }

    peer_pieces[peer_id][piece_id] = true;
}

/* updates the pieces of a given peer based on a bitfield */
void handle_bitfield(std::string peer_id, unsigned long bitfield) {
    peer_pieces[peer_id] = dynamic_bitset(num_pieces, bitfield);
}

/* returns the pieces that the client has and we're missing */
boost::dynamic_bitset PieceManager::missing_pieces(string peer_id) {
    if(peer_pieces.find(peer_id) == peer_pieces.end()) {
        return dynamic_bitset(num_pieces);
    }
    return (peer_pieces[peer_id] - my_pieces);
}
