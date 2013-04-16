#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <cstdlib>

#include <boost/shared_ptr.hpp>
#include <boost/dynamic_bitset.hpp>

#include "common.h"

#ifndef PIECE_H
#define PIECE_H

/* Keeps track the pieces we have and need */
class PieceManager {
public:
    PieceManager(int num_pieces, int piece_size);

    // operations on client pieces
    void update_piece(int have_piece);
    void update_pieces(boost::dynamic_bitset bitfield);
    unsigned long pieces_to_bitfield();

    // operations on peer pieces
    void handle_have(std::string peer_id, int piece_id);
    void handle_bitfield(std::string peer_id, unsigned long bitfield, size_t n);

    boost::dynamic_bitset missing_pieces(std::string peer_id);


private:
    int num_pieces;
    int piece_size;
    boost::dynamic_bitset my_pieces;
    std::map<string, boost::dynamic_bitset> peer_pieces;
};
#endif
