fredtorrent
===========
An implementation of the BitTorrent Protocol

It will implement the Peer-to-peer and Peer-to-tracker protocol. The goal is to
have a commandline program that can succesfuly download Ubuntu via BitTorrent:

    http://www.ubuntu.com/download/desktop/alternative-downloads

# Steps
1. Implement Peer-to-tracker protocol in library
2. Implement Peer-to-peer protocol in library
3. Create client program using library

# Design

(BTSession)
    1. Read metainfo file via. (MetaInfoFile)
        1.1 Use (Bencode) to parse

    2. Use data from (MetaInfoFile) for (TrackerConnection)
        2.1 Initialize request settings
        2.2 get and store peer list

    3. Connect to a few peers via. (PeerConnection)

# TODO
2. create Bencode class
3. create MetaInfoFile class
4. create BTSession class (top class encompassing everything)

5. Finish TrackerConnection class
5. Create Peer, PeerList classes
5. Create BTFile, PieceManager classes
    - which pieces I already have
    - who has which piece
    - frequency of pieces

6. create PeerConnection class
    - handshake, keep-alive, choke, unchoke, interested, uninterested
    - have, request, piece

# Dependencies
libcurl

    curl-config --libs
