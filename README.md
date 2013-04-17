fredtorrent
===========
An implementation of the BitTorrent Protocol.

It implements the Peer-Tracker and and Peerwire protocols. The program is
written in C++ and runs on a Linux environment.


To compile everything do:

    make main

However, this does not run yet (see TODO).

# Dependencies
libboost
libcurl


# Files

common.h
    - contains functions and contants used globally

bencode.h
    - contains functions to parse bencoded strings

metainfo.h
    - contains functions to parse the metainfo file

peer.h
    - classes to represent peers and to manage them

piece.h
    - classes to represent pieces of the file

tracker.h
    - functions to announce to the tracker

peerwire.h
    - functions to send messages in the peerwire protocol

btsession.h
    - top level file 
    - takes metainfo filepath as argument
    - parse metainfo file
    - announce to tracker
    - opens connections with peers and listens for new connections
    - receives messages and handles them


# TODO: 

Currently, the fredtorrent does not work. It can parse the metainfo file and
announce to the tracker. I have also implemented methods to send the peerwire
protocol messages and a basic framework for handling new and existing
connections from the peers.

However, I still need to "pipe" everything together and handle the each of the
messages properly. I also need to test it on an actual tracker with actual
peers.

Here are some of the significant things to finish:
1. close open connections to peers when timed out
    - open conn. to new peers to replace old ones

2. send request messages to peer when unchoked and interested

3. function to write pieces recieved to disk for piece msg

4. functions to respond to other request msg
    - read pieces from disk and send piece msg

I am unsure whether or not to continue with this project, maybe.
