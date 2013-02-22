#include "bencode.h"

/* parses a bencoded string and returns the right token  */
// TODO: use reference rather than return token straight up ?
// TODO: get around object slicing problem
BencodeToken parseBencodeToken(char * content, size_t length) {
    switch(*content) {
        case 'i':
            return BencodeInteger(content, length);
            break;
        case 'l':
            return BencodeList(content, length);
            break;
        case 'd':
            return BencodeDictionary(content, length);
            break;
        default:
           return BencodeString(content, length);
           break;
    }
}

/* parses a bencoded string and returns a list of BencodeTokens  */
// TODO: use reference rather than return container straight up ?
// TODO: get around object slicing problem
std::vector<BencodeToken> parseBencode(char *content, size_t length) {
    char *end = content + length;
    std::vector<BencodeToken> tokList;

    // Iterate through each string and create tokens
    while(content < end) {
        BencodeToken tok = parseBencodeToken(content, length);
        tokList.push_back(tok);
        content += tok.char_length;
        length -= tok.char_length;
    }
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
    this->type = BE_STRING;
    this->char_length = num_size_chars + 2 + size;
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
    long int size = atoi(s.c_str());
    this->type = BE_INTEGER;
    // 2 for 'i:' and 1 for 'e:'
    this->char_length = 2 + num_int_chars + 1;
}

BencodeList::BencodeList(char *content, size_t length) : value() {
    assert(content[0] == 'l' && content[1] == ':');

    // jump past the 'l:' tokens
    this->raw_string = content;
    content += 2;
    while(*content != 'e') {
        BencodeToken tok = parseBencodeToken(content, length);
        this->value.push_back(tok);
        content += tok.char_length;
        length -= tok.char_length;
    }

    this->type = BE_LIST;
    this->char_length = (content - this->raw_string) + 1;
}

BencodeDictionary::BencodeDictionary(char *content, size_t length) : value() {
    assert(content[0] == 'd' && content[1] == ':');

    // jump past the 'd:' tokens
    this->raw_string = content;
    content += 2;
    while(*content != 'e') {
        BencodeString key = BencodeString(content, length);
        content += key.char_length;
        length -= key.char_length;
        assert(key.type = BE_STRING);

        BencodeToken val = parseBencodeToken(content, length);
        content += val.char_length;
        length -= val.char_length;
        this->value.insert(std::pair<std::string, BencodeToken>(key.value, val));
    }
    this->type = BE_DICT;
    this->char_length = (content - this->raw_string) + 1;
}
