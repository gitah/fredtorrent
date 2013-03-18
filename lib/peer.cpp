Peer Peer::parse(map<std::string, BencodeTokenPtr> peer_dict) {
    // extracts information from the peer dictionary in the tracker response
    // creates a Peer object from this information
    Peer peer;
    peer.peer_id = BencodeString.get_value(resp_dict["peer id"]);
    peer.ip = BencodeString.get_value(resp_dict["ip"]);
    peer.port = BencodeInteger.get_value(resp_dict["port"]);
    return peer;
}
