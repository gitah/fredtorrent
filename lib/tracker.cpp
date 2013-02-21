TrackerConnection::TrackerConnection(char *host, uint32_t port) {
    mHost = string(host);
    mPort = port;

    mConn = curl_easy_init();
    if(curl) {
        curl_easy_setopt(mConn, CURLOPT_URL, host);
        curl_easy_setopt(mConn, CURLOPT_PORT, port);
        curl_easy_setopt(mConn, CURLOPT_USERAGENT, BT_UA);
    }
}
