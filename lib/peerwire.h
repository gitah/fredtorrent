/* peerwire.h
 *
 * Contains functions to send peerwire messages 
 *
 * */

#include <string>
#include <bitset>

#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "common.h"

#ifndef PEERWIRE_H
#define PEERWIRE_H


#define PSTR 'BitTorrent protocol'
#define PSTRLEN 19
#define REQUEST_MSG_BLOCK_LEN (1 << 14)

#define HANDSHAKE_RESERVED_SIZE 8
#define HANDSHAKE_MSG_SIZE PSTRLEN+49
#define LENGTH_PREFIX_SIZE 4
#define MSG_ID_SIZE 1
#define REQUEST_MSG_SIZE 13

#define CHOKE_MSG_ID            0x00
#define UNCHOKE_MSG_ID          0x01
#define INTERESTED_MSG_ID       0x02
#define UNINTERESTED_MSG_ID     0x03
#define HAVE_MSG_ID             0x04
#define BITFIELD_MSG_ID         0x05
#define REQUEST_MSG_ID          0x06
#define PIECE_MSG_ID            0x07



// creates a TCP socket to the specified destination
// returns the socket descriptor if succesful otherwise NULL
int init_connection(std::string dst_ip, int dst_port);

// sends a handshake message
bool send_handshake(int sockfd, std::string peer_id, std::string info_hash);

// sends messages to update peer state
bool send_choke(int sockfd);
bool send_unchoke(int sockfd);
bool send_interested(int sockfd);
bool send_uninterested(int sockfd);

//TODO: create PieceManager before using these !!!
// sends messages to indicate pieces available
bool send_have(int sockfd);
bool send_bitfield(int sockfd);

// send messages to request and get data
bool send_request(int sockfd);
bool send_piece(int sockfd);

#endif