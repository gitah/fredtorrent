#ifndef BENCODE_H
#define BENCODE_H

#include <string>
#include <vector>
#include <cstdlib>
#include "constants.h"

enum TOKEN_TYPE {
    BE_STRING, BE_INTEGER, BE_LIST, BE_DICT
}

class BencodeToken {
public:
    const char *raw_string;
    const size_t char_length;
    const TOKEN_TYPE type;
}

class BencodeString : BencodeToken {
public:
    BencodeString(char *content, size_t length);
    const std::string value;
}

class BencodeInteger : BencodeToken {
public:
    BencodeInteger(char *content, size_t length);
    long int value;
}

class BencodeList : BencodeToken {
    BencodeList(char *content, size_t length);
    std::vector<BencodeToken> value;
}

class BencodeDictionary : BencodeToken {
    BencodeInteger(char *content, size_t length);
    std::map<BencodeString, BencodeToken> value;
}


std::vector parseBencode(char *content, size_t length);

#endif
