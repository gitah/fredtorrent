#include "bencode.h"

/* parses a bencoded string and returns the right token  */
BencodeTokenPtr parseBencodeToken(char * content, size_t length) {
    switch(*content) {
        case 'i':
            return BencodeTokenPtr(new BencodeInteger(content, length));
        case 'l':
            return BencodeTokenPtr(new BencodeList(content, length));
        case 'd':
            return BencodeTokenPtr(new BencodeDictionary(content, length));
        default:
           if(*content > '0' && *content <= '9') {
               return BencodeTokenPtr(new BencodeString(content, length));
           } else {
               break;
           }
    }
    return boost::shared_ptr<BencodeToken>();
}

/* parses a bencoded string and returns a list of BencodeTokens  */
std::vector<BencodeTokenPtr> parseBencode(char *content, size_t length) {
    char *end = content + length;
    std::vector<BencodeTokenPtr> toks;

    // Iterate through each string and create tokens
    while(content < end) {
        BencodeTokenPtr tok = parseBencodeToken(content, length);
        toks.push_back(tok);
        content += tok->char_length;
        length -= tok->char_length;
    }
    return toks;
}

BencodeString::BencodeString(char *content, size_t length) : value() {
    // find out how long the string is
    this->raw_string = content;
    int num_size_chars = 0;
    while(*(content + num_size_chars) != ':') {
        num_size_chars++;
    }
    std::string s(content, num_size_chars);
    long int size = atoi(s.c_str());

    // skip past the ':' and extract string
    this->value = std::string(content+2, (int)size);
    //std::cout <<  "s: " << this->value << std::endl;
    this->type = BE_STRING;
    this->char_length = num_size_chars + 1 + size;
}

BencodeInteger::BencodeInteger(char *content, size_t length) : value() {
    assert(content[0] == 'i' && content[1] == ':');
    this->raw_string = content;

    // jump past the 'i:' tokens
    content += 2;
    size_t num_int_chars = 0;
    while(content[num_int_chars] != 'e') {
        num_int_chars++;
    }
    std::string s(content, num_int_chars);

    this->value = atoi(s.c_str());
    this->type = BE_INTEGER;
    // 2 for 'i:' and 1 for 'e:'
    this->char_length = 2 + num_int_chars + 1;
}

BencodeList::BencodeList(char *content, size_t length) : value() {
    assert(*content == 'l');

    this->raw_string = content;
    content += 1; // jump past the 'l' char
    while(*content != 'e') {
        //std::cout <<  "l: " << *content << std::endl;
        BencodeTokenPtr tok = parseBencodeToken(content, length);
        this->value.push_back(tok);
        content += tok->char_length;
        length -= tok->char_length;
    }

    this->type = BE_LIST;
    this->char_length = (content - this->raw_string) + 1;
}

BencodeDictionary::BencodeDictionary(char *content, size_t length) : value() {
    assert(*content == 'd');

    this->raw_string = content;
    content += 1; // jump past the 'd' char
    while(*content != 'e') {
        //std::cout <<  "d: " << *content << std::endl;
        BencodeString key_str = BencodeString(content, length);
        content += key_str.char_length;
        length -= key_str.char_length;
        assert(key_str.type == BE_STRING);
        std::string key = key_str.value;

        BencodeTokenPtr val = parseBencodeToken(content, length);
        content += val->char_length;
        length -= val->char_length;
        this->value.insert(std::pair<std::string, BencodeTokenPtr>(key, val));
    }
    this->type = BE_DICT;
    this->char_length = (content - this->raw_string) + 1;
}
