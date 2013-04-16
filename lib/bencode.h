#ifndef BENCODE_H
#define BENCODE_H

#include <string>
#include <vector>
#include <map>
#include <cstdlib>

#include <boost/shared_ptr.hpp>

#include "common.h"

enum TOKEN_TYPE {
    BE_STRING, BE_INTEGER, BE_LIST, BE_DICT
};

class BencodeToken {
public:
    const char *raw_string;
    size_t char_length;
    TOKEN_TYPE type;


    static BencodeTokenPtr parseBencodeToken(const char * content, size_t length);
    static std::vector<BencodeTokenPtr> parseBencode(const char *content, size_t length);
};


// TODO: support unicode (decode UTF-8)
class BencodeString : public BencodeToken {
public:
    BencodeString(const char *content, size_t length);
    std::string value;

    // extracts value from a BencodeTokenPtr
    static std::string get_value(BencodeTokenPtr ptr);
};

class BencodeInteger : public BencodeToken {
public:
    BencodeInteger(const char *content, size_t length);
    long int value;

    // extracts value from a BencodeTokenPtr
    static long int get_value(BencodeTokenPtr ptr);
};

class BencodeList : public BencodeToken {
public:
    BencodeList(const char *content, size_t length);
    std::vector<BencodeTokenPtr> value;

    // extracts value from a BencodeTokenPtr
    static std::vector<BencodeTokenPtr> get_value(BencodeTokenPtr ptr);
};

class BencodeDictionary : public BencodeToken {
public:
    BencodeDictionary(const char *content, size_t length);
    std::map<std::string, BencodeTokenPtr> value;

    // extracts value from a BencodeTokenPtr
    static std::map<std::string, BencodeTokenPtr> get_value(BencodeTokenPtr ptr);
};

#endif
