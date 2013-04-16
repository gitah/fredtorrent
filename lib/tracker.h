#ifndef TRACKER_H
#define TRACKER_H

#include <time.h>
#include <curl/curl.h>

#include "bencode.h"
#include "peer.h"
#include "common.h"

class TrackerRequest;
class TrackerResponse;
class TrackerConnection;

// contains state information regarding a client and its tracker
class TrackerConnection {
public:
    TrackerConnection(std::string tracker_url, int port);

    // announces client to tracker
    std::vector<Peer> announce(TrackerRequest req);

    // checks if we can reannounce to tracker
    // true if the last sent time + interval > current time
    bool interval_expired();

private:
    TrackerResponse send_request(TrackerRequest req, std::string event = "");

    std::string mTrackerURL;
    int mPort;

    // time stamp when we last sent a request to tracker
    time_t mLastSent;

    // tracker information
    int mInterval;
    std::string mTrackerId;
};

// contains information to send to tracker
class TrackerRequest {
public:
    // urlencoded 20-byte SHA1 hash of the value of the info key from the Metainfo file.
    std::string info_hash;

    // urlencoded 20-byte string used as a unique ID for the client,
    std::string peer_id;

    std::string tracker_id;

    // The port number that the client is listening on.
    int client_port;

    //  The total amount of bytes uploaded
    int uploaded;

    //  The total amount of bytes downloaded
    int downloaded;

    //  The total amount of bytes remaining in file
    int left;

    // if event is "started" the request will indicate it to the tracker
    // if event is "completed" the request will indicate it to the tracker
    // all other values of event will not do anything
    std::string event;
};

// contains information received from tracker
class TrackerResponse {
public:
    static TrackerResponse parse(std::map<std::string, BencodeTokenPtr> resp_dict);

    // If present, then no other keys may be present. The value is a
    // human-readable error message as to why the request failed (string).
    std::string failure_reason; 

    // seconds that the client should wait between sending
    // regular requests to the tracker
    int interval; 

    //A string that the client should send back on its next announcements. If
    //absent and a previous announce sent a tracker id, do not discard the old
    //value; keep using it.
    std::string tracker_id;

    // number of of seeds
    int complete;

    // number of leechers
    int incomplete;

    // list of peers
    std::vector<Peer> peers;
};
#endif
