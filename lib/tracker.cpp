#include "tracker.h"

using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream)
{
  size_t realsize = size * nmemb;
  std::string temp(static_cast<const char*>(ptr), realsize);
  stream.append(temp);
  return realsize;
}

/* === TrackerConnection === */
TrackerConnection::TrackerConnection() : mTrackerURL(""), mPort(0) {}

TrackerConnection::TrackerConnection(string tracker_url, int port) : mLastSent(0), mInterval(0), mTrackerId("") {
    mTrackerURL = tracker_url;
    mPort = port;
}

/* announces client to tracker
 * update connection state from information obtained from tracker
 *
 * returns list of peers obtained from tracker
 * returns empty list if failure
 * */
vector<Peer> TrackerConnection::announce(TrackerRequest req) {
    // fill in missing info from request
    // indicate this is the initial connection if we have not announced before
    req.event = (mLastSent > 0) ? "" : "started";

    if(!mTrackerId.empty()) {
        req.tracker_id = this->mTrackerId;
    }


    TrackerResponse resp = send_request(req);
    if(!resp.failure_reason.empty()) {
        return vector<Peer>();
    }

    this->mInterval = resp.interval;
    this->mTrackerId = resp.tracker_id;

    this->mLastSent = time(NULL);

    return resp.peers;
}

/* returns true if the last announce time was greater than the tracker */
bool TrackerConnection::interval_expired() {
    // we have not announced to tracker yet
    if(mInterval <= 0 || mLastSent == 0) {
        return false;
    }
    return (int)time(NULL) > ((int)mLastSent + mInterval);
}

/* sends an HTTP request to the tracker and returns the response */
TrackerResponse TrackerConnection::send_request(TrackerRequest req, string event) {
    // build query string from TrackerRequest
    string query_str = string("?") +
        string("info_hash=") + req.info_hash +
        string("&peer_id=") + req.peer_id +
        string("&port=") + to_string(req.client_port) +
        string("&uploaded=") + to_string(req.uploaded) +
        string("&downloaded=") + to_string(req.downloaded) +
        string("&left=") + to_string(req.left) +
        string("&compact=0");
    if(!req.tracker_id.empty()) {
        query_str += "&trackerid=" + req.tracker_id;
    }
    if(event == "started" || event == "completed") {
        query_str += "&event=" + string(req.event);
    }
    string url = mTrackerURL + string(":") + to_string(mPort) + query_str;

    // send the request to tracker
    string response;
    CURL *curl = curl_easy_init();
    if(curl) {
        //curl_easy_setopt(curl, CURLOPT_PORT, TRACKER_PORT);
        curl_easy_setopt(curl, CURLOPT_PORT, mPort);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, BT_UA);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(res != CURLE_OK) {
            TrackerResponse resp;
            resp.failure_reason = "HTTP request failed";
            return resp;
        }
    } else {
        TrackerResponse resp;
        resp.failure_reason = "Could create CURL handle";
        return resp;
    }

    // parse and return TrackerResponse
    vector<BencodeTokenPtr> toks = BencodeToken::parseBencode(response.c_str(), response.length());
    assert(toks.size() == 1);
    assert(toks[0]->type == BE_DICT);
    return TrackerResponse::parse(BencodeDictionary::get_value(toks[0]));
}


/* === TrackerResponse === */
TrackerResponse TrackerResponse::parse(std::map<std::string, BencodeTokenPtr> resp_dict) {
    // extracts information from the dictionary returned by tracker response
    TrackerResponse tr;
    if(resp_dict.find("failure reason") != resp_dict.end()) {
        tr.failure_reason = BencodeString::get_value(resp_dict["failure reason"]);
    }
    if(resp_dict.find("tracker id") != resp_dict.end()) {
        tr.tracker_id = BencodeString::get_value(resp_dict["tracker id"]);
    }
    tr.interval = BencodeInteger::get_value(resp_dict["interval"]);
    tr.complete = BencodeInteger::get_value(resp_dict["complete"]);
    tr.incomplete = BencodeInteger::get_value(resp_dict["incomplete"]);

    // extract peer information
    vector<BencodeTokenPtr> peer_dicts = BencodeList::get_value(resp_dict["peers"]);
    for(int i=0; i<peer_dicts.size(); i++) {
        map<string, BencodeTokenPtr> peer_dict = BencodeDictionary::get_value(peer_dicts[i]);
        tr.peers.push_back(Peer::parse(peer_dict));
    }

    return tr;
}
