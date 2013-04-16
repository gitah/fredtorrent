#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

#include <boost/shared_ptr.hpp>

#define BT_UA "FT"
#define CLIENT_PORT 6885
#define TRACKER_PORT 80

#define SHA1_LEN 20
#define PEER_ID_LEN 20

typedef uint32_t BT_ID;

// forward declarations
class BencodeToken;

// forward typedefs
typedef boost::shared_ptr<BencodeToken> BencodeTokenPtr;


// convert integer to string
inline std::string to_string(int num) {   
    std::stringstream ss;
    ss << num;
    return ss.str();
}


#endif
