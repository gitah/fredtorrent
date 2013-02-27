#ifndef BENCODE_H
#define BENCODE_H

#include <string>
#include <vector>
#include <map>
#include <cstdlib>

#include <boost/shared_ptr.hpp>

#include "constants.h"

enum TOKEN_TYPE {
    BE_NONE, BE_STRING, BE_INTEGER, BE_LIST, BE_DICT
};

class BencodeToken {
public:
    const char *raw_string;
    size_t char_length;
    TOKEN_TYPE type;
};

typedef boost::shared_ptr<BencodeToken> BencodeTokenPtr;

class BencodeString : public BencodeToken {
public:
    BencodeString(char *content, size_t length);
    std::string value;
};

class BencodeInteger : public BencodeToken {
public:
    BencodeInteger(char *content, size_t length);
    long int value;
};

class BencodeList : public BencodeToken {
public:
    BencodeList(char *content, size_t length);
    std::vector<BencodeTokenPtr> value;
};

class BencodeDictionary : public BencodeToken {
public:
    BencodeDictionary(char *content, size_t length);
    std::map<std::string, BencodeTokenPtr> value;
};

BencodeTokenPtr parseBencodeToken(char * content, size_t length);
std::vector<BencodeTokenPtr> parseBencode(char *content, size_t length);
#endif
