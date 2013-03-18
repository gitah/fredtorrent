#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <cassert>
#include <cstring>
#include <iostream>
#include <boost/shared_ptr.hpp>

#define BT_UA "fredtorrent"

#define SHA1_LEN 20

typedef uint32_t BT_ID;

// forward declarations
class BencodeToken;

// forward typedefs
typedef boost::shared_ptr<BencodeToken> BencodeTokenPtr;


#endif
