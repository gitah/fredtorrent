#ifndef TRACKER_H
#define TRACKER_H

#include <iostream>
#include <string>

#include <curl/curl.h>

#include "constants.h"

class TrackerConnection {
public:
    TrackerConnection(MetaInfoFile meta);
    int getPeers();
private:
    CURL * mConn;
}
#endif
