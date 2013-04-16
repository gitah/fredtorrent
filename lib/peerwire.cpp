#include "peerwire.h"

using namespace std;

int init_connection(string dst_ip, int dst_port) {
    // create TCP socket
    int sockfd = 0;
    if((sockfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        return 0;
    }

    //set socket to be reusable
    //int yes = 1;
    //if (setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(int)) == -1) {
        //return 0;
    //}

    // create `struct sockaddr` for the peer address
    struct sockaddr_in peer_addr;
    socklen_t addr_size = sizeof(peer_addr);
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(dst_port);
    inet_pton(AF_INET, dst_ip.c_str(), &(peer_addr.sin_addr));

    // connect to peer
    if(connect(sockfd, (struct sockaddr *)&peer_addr, addr_size) == -1) {
        return 0;
    }

    return sockfd;
}

bool send_handshake(int sockfd, std::string peer_id, std::string info_hash) {
    // handshake: <pstrlen><pstr><reserved><info_hash><peer_id>
    uint8_t msg[HANDSHAKE_MSG_SIZE];
    uint8_t *offset = msg;

    // PSTRLEN
    *(offset) = PSTRLEN;
    offset += 1;

    // PSTR
    memcpy(offset, PSTR, PSTRLEN);
    offset += PSTRLEN;

    //RESERVED
    for(int i=0; i<HANDSHAKE_RESERVED_SIZE; i++) {
        (*offset) = 0x00;
        offset += 1;
    }

    // INFO_HASH
    memcpy(offset, info_hash.c_str(), SHA1_LEN);
    offset += SHA1_LEN;

    // INFO_HASH
    memcpy(offset, peer_id.c_str(), PEER_ID_LEN);

    // send message
    if(send(sockfd, msg, HANDSHAKE_MSG_SIZE, 0) == -1) {
        return false;
    }
    return true;
}

/*-- peer state messages --*/

// convenience function to set up the message for choke/unchoke and
// interested/uninterested
void init_short_msg(uint8_t *msg, uint8_t msg_id) {
    // length prefix
    *((uint32_t *)msg) =  htonl(MSG_ID_SIZE);
    msg += sizeof(uint32_t);

    // msg id
    *(msg) = msg_id;
}

bool send_choke(int sockfd) {
    // choke: <len=0001><id=0>
    int msg_size = LENGTH_PREFIX_SIZE + MSG_ID_SIZE;
    uint8_t msg[msg_size];
    init_short_msg(msg, CHOKE_MSG_ID);

    // send message
    if(send(sockfd, msg, msg_size, 0) == -1) {
        return false;
    }
    return true;
}

bool send_unchoke(int sockfd) {
    // unchoke: <len=0001><id=1>
    int msg_size = LENGTH_PREFIX_SIZE + MSG_ID_SIZE;
    uint8_t msg[msg_size];
    init_short_msg(msg, UNCHOKE_MSG_ID);

    // send message
    if(send(sockfd, msg, msg_size, 0) == -1) {
        return false;
    }
    return true;
}

bool send_interested(int sockfd) {
    // interested: <len=0001><id=2>
    int msg_size = LENGTH_PREFIX_SIZE + MSG_ID_SIZE;
    uint8_t msg[msg_size];
    init_short_msg(msg, INTERESTED_MSG_ID);

    // send message
    if(send(sockfd, msg, msg_size, 0) == -1) {
        return false;
    }
    return true;
}

bool send_uninterested(int sockfd) {
    // uninterested: <len=0001><id=3>
    int msg_size = LENGTH_PREFIX_SIZE + MSG_ID_SIZE;
    uint8_t msg[msg_size];
    init_short_msg(msg, UNINTERESTED_MSG_ID);

    // send message
    if(send(sockfd, msg, msg_size, 0) == -1) {
        return false;
    }
    return true;
}

/*-- piece available messages --*/
bool send_have(int sockfd) {
    return false;
}
bool send_bitfield(int sockfd) {
    return false;
}

/*-- piece transfer messages --*/
bool send_request(int sockfd, uint32_t index, uint32_t begin) {
    // request: <len=0013><id=6><index><begin><length>
    // index: integer specifying the zero-based piece index
    // begin: integer specifying the zero-based byte offset within the piece
    // length: integer specifying the requested length.
    //      NOTE: length is always 2^14
    uint32_t msg_size = LENGTH_PREFIX_SIZE + REQUEST_MSG_SIZE;
    uint8_t msg[msg_size];
    uint8_t *offset = msg;

    // MESSAGE LENGTH
    *((uint32_t *)offset) =  htonl(REQUEST_MSG_SIZE);
    offset += sizeof(uint32_t);

    // MSG ID
    *(offset) = REQUEST_MSG_ID;
    offset += 1;

    // PIECE INDEX
    *((uint32_t *)offset) =  htonl(index);
    offset += sizeof(uint32_t);

    // BEGIN
    *((uint32_t *)offset) =  htonl(begin);
    offset += sizeof(uint32_t);

    // DATA LENGTH
    *((uint32_t *)offset) =  htonl(REQUEST_MSG_BLOCK_LEN);
    offset += sizeof(uint32_t);

    // send message
    if(send(sockfd, msg, msg_size, 0) == -1) {
        return false;
    }
    return true;
}

bool send_piece(int sockfd, uint32_t index, uint32_t begin, const char *data, size_t n) {
    //piece: <len=0009+X><id=7><index><begin><block>
    //The piece message is variable length, where X is the length of the block. The payload contains the following information:
    //index: integer specifying the zero-based piece index
    //begin: integer specifying the zero-based byte offset within the piece
    //block: block of data, which is a subset of the piece specified by index.
    uint32_t msg_size = LENGTH_PREFIX_SIZE + 9 + n;
    uint8_t msg[msg_size];
    uint8_t *offset = msg;

    // MESSAGE LENGTH
    *((uint32_t *)offset) =  htonl(9 + n);
    offset += sizeof(uint32_t);

    // MSG ID
    *(offset) = PIECE_MSG_ID;
    offset += 1;

    // PIECE INDEX
    *((uint32_t *)offset) =  htonl(index);
    offset += sizeof(uint32_t);

    // BEGIN
    *((uint32_t *)offset) =  htonl(begin);
    offset += sizeof(uint32_t);

    // DATA LENGTH
    memcpy(offset, data, n);

    // send message
    if(send(sockfd, msg, msg_size, 0) == -1) {
        return false;
    }
    return true;
}
