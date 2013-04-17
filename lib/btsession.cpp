#include "btsession.h"

using namespace std;

// TODO: 
//  1. close open connections on timeout
//      - open conn. to new peers to replace old ones
//
//  2. finish message handlers:
//      - piece message
//      - request message
//
//  3. write class to write piece to disk

BTSession::BTSession(std::string metainfo_file) :
    mMetainfo(metainfo_file), mPeerManager() {

    // read metainfo file
    mMetainfo = MetaInfo(metainfo_file);
    int num_pieces = mMetainfo.pieces.size();
    mPieceManager = PieceManager(num_pieces, mMetainfo.piece_length);

    // connect to tracker
    mTrackerConnection = TrackerConnection(mMetainfo.tracker_url, TRACKER_PORT);
    std::vector<Peer> peer_list = this->get_peers();

    // connect to peers
    for(int i; i<peer_list.size(); i++) {
        this->connect_peer(peer_list[i]);
        if(open_connections.size() >= MAX_ACTIVE_PEER_CONN)
            break;
    }

    // create a connection to listen to new peers
    listen_conn = init_incoming_connection(CLIENT_PORT);
    if(listen(listen_conn, 5) == -1) {
        perror("could not listen to port");
        exit(1);
    }
    open_connections.push_back(listen_conn);

    // listen to messages from existing peers and messages from new peers
    while(true) {
        this->listen_incoming_msg();
    }
}

/* opens a connection to the given Peer */
void BTSession::connect_peer(Peer p) {
    int sockfd = init_outgoing_connection(p.ip, p.port);

    // send handshake
    bool succ = send_handshake(sockfd, p.peer_id, mMetainfo.info_hash);
    if(!succ) return;

    this->open_connections.push_back(sockfd);
    mPeerManager.add_peer(sockfd, p.peer_id, p.ip, p.port);

    // send bitfield
    unsigned long bitfield = mPieceManager.pieces_to_bitfield();
    int num_pieces_bytes = mMetainfo.pieces.size() / CHAR_BIT + 1;
    succ = send_bitfield(sockfd, (uint8_t *)&bitfield, num_pieces_bytes);
    if(!succ) return;
}

/* sends a request to tracker and updates state information based on tracker
 * response
 *
 * returns true if success otherwise false
 * */
std::vector<Peer> BTSession::get_peers() {
    //TrackerRequest req = this->createTrackerRequest();
    TrackerRequest req; // TODO
    return mTrackerConnection.announce(req);
}

/* creates a TrackerRequest from members to send to tracker */
TrackerRequest BTSession::create_tracker_request(std::string event_type) {
    TrackerRequest req;

    req.peer_id = this->peer_id;
    req.info_hash = mMetainfo.info_hash;   // TODO: url encode
    req.client_port = CLIENT_PORT;

    //  The total amount of bytes uploaded
    req.uploaded = this->bytes_uploaded;
    req.downloaded = this->bytes_uploaded;
    req.left = this->bytes_left;

    return req;
}


/* processes an incoming message */
void BTSession::handle_msg(int sockfd, uint8_t *msg, size_t n, string src_ip, int src_port) {
    // check handshake: message right size and connection not found
    if(n == HANDSHAKE_MSG_SIZE && sockfd == listen_conn) {
        // new connection from peer
        string peer_id((char *)(msg + HANDSHAKE_MSG_SIZE - PEER_ID_LEN), PEER_ID_LEN);
        mPeerManager.add_peer(sockfd, peer_id, src_ip, src_port);
        this->open_connections.push_back(sockfd);
    }

    // existing connection
    // message is too short
    if(n < 5) return;

    // find message id
    uint8_t msg_id = msg[4];
    switch(msg_id) {
        case CHOKE_MSG_ID:
            mPeerManager.handle_choke(sockfd);
            break;
        case UNCHOKE_MSG_ID:
            mPeerManager.handle_unchoke(sockfd);
            break;
        case INTERESTED_MSG_ID:
            mPeerManager.handle_interested(sockfd);
            break;
        case UNINTERESTED_MSG_ID:
            mPeerManager.handle_uninterested(sockfd);
            break;
        case HAVE_MSG_ID:
            break;
        case BITFIELD_MSG_ID:
            break;
        case REQUEST_MSG_ID:
            break;
        case PIECE_MSG_ID:
            break;
        default:
            break;
    }
}


/* blocks until we receive a message, then processes it */
void BTSession::listen_incoming_msg() {
    int max_fd = -1;
    fd_set readfds;
    FD_ZERO(&readfds);
    for(int i=0; i<open_connections.size(); i++) {
        int sockfd = open_connections[i];
        FD_SET(sockfd, &readfds);
        if(sockfd > max_fd) {
            max_fd = sockfd;
        }
    }
    struct timeval tv;
    tv.tv_sec = 2;

    int ready = select(max_fd+1, &readfds, NULL, NULL, &tv);
    if(ready == -1) return;

    struct sockaddr_storage src_addr;
    socklen_t addrlen = sizeof(src_addr);
    int new_sockfd = accept(ready, (struct sockaddr *)&src_addr, &addrlen); 
    if(new_sockfd == -1) return;

    // get ip address of the source host
    struct in_addr ip_network = ((struct sockaddr_in *)&src_addr)->sin_addr;
    char ip_host[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip_network, ip_host, INET6_ADDRSTRLEN);
    string src_ip(ip_host);

    // get port of the source host
    unsigned short src_port = ((struct sockaddr_in *)&src_addr)->sin_port;
    src_port = ntohs(src_port);
        
    int msg_len = 0;
    ioctl(new_sockfd, FIONREAD, &msg_len);
    char buffer[msg_len];
    read(new_sockfd, buffer, msg_len);
    handle_msg(new_sockfd, (uint8_t *)buffer, msg_len, src_ip, src_port);
}


int main() {
}
