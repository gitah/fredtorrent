using namespace std;

size_t write_to_string(void *ptr, size_t size, size_t nmemb, std::string stream)
{
  size_t realsize = size * nmemb;
  std::string temp(static_cast<const char*>(ptr), realsize);
  stream.append(temp);
  return realsize;
}

/* TrackerConnection */
TrackerConnection::TrackerConnection(string tracker_url) {
    mTrackerURL = tracker_url;
}

TrackerResponse TrackerConnection::send_request(TrackerRequest req) {
    // build query string from TrackerRequest
    string query_str = "?" + 
        "info_hash=" + string(req.info_hash) + "&" + 
        "peer_id=" + string(req.peer_id) + "&" + 
        "port=" + string(req.port) + "&" + 
        "uploaded=" + string(req.uploaded) + "&" + 
        "downloaded=" + string(req.downloaded) + "&" + 
        "left=" + string(req.left) + "&" + 
        "compact=0";
    if(event == "started" || event == "completed") {
        query_str += "&event=" + string(req.event);
    }

    // send the request to tracker
    string response;
    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_PORT, TRACKER_PORT);
        curl_easy_setopt(curl, CURLOPT_USERAGENT, BT_UA);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        curl_easy_setopt(curl, CURLOPT_URL, mTrackerURL + query_str);

        CURLcode res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        if(res != CURLE_OK) {
            return vector();
        }
    }
 
    // parse and return TrackerResponse
    vector<BencodeTokenPtr> toks = BencodeToken::parseBencodeToken(response.c_str(), response.length());
    assert(toks.size(), 1);
    assert(toks[0].type, BE_DICT);
    return TrackerResponse::parse(BencodeDictionary::get_value(toks[0]));
}

/* TrackerResponse */
TrackerResponse TrackerResponse::parse(map<std::string, BencodeTokenPtr> resp_dict) {
    // extracts information from the dictionary returned by tracker response
    TrackerResponse tr;
    tr.failure_reason = BencodeString.get_value(resp_dict["failure reason"]);
    tr.interval = BencodeInteger.get_value(resp_dict["interval"]);
    tr.tracker_id = BencodeString.get_value(resp_dict["tracker id"]);
    tr.complete = BencodeInteger.get_value(resp_dict["complete"]);
    tr.incomplete = BencodeInteger.get_value(resp_dict["incomplete"]);

    // extract peer information
    vector<BencodeTokenPtr> peer_dicts = BencodeList::get_value(resp_dict["peers"]);
    for(int i=0; i<peer_dicts.size(); i++) {
        map<string, BencodeTokenPtr> peer_dict = peer_dicts[i];
        tr.peers.push_back(Peer::parse(peer_dict));
    }

    return tr;
}
