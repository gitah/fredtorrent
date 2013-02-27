/*Tests bencoding function works correctly*/

#include <cassert>
#include <cstring>
#include <vector>
#include <iostream>
#include "bencode.h"

void test_bencode_string() {
    char tok[] = "3:foo";
    std::vector<BencodeTokenPtr> lst = parseBencode(tok, strlen(tok));
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_STRING);
    assert(
        ((BencodeString *)lst[0].get())->value == "foo"
    );
}

void test_bencode_integer() {
    char tok[] = "i:200e";
    std::vector<BencodeTokenPtr> lst = parseBencode(tok, strlen(tok));

    assert(lst.size() == 1);
    assert(lst[0]->type == BE_INTEGER);

    int val = ((BencodeInteger *)lst[0].get())->value;
    assert(val == 200);
}

void test_bencode_list() {
    char tok[] = "l4:spam4:eggse";
    std::vector<BencodeTokenPtr> lst = parseBencode(tok, strlen(tok));
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_LIST);

    std::vector<BencodeTokenPtr> blst = ((BencodeList *)lst[0].get())->value;
    std::cout << "foo " << blst.size() << std::endl;
    assert(blst.size() == 2);
    assert(((BencodeString *)blst[0].get())->value == "spam");
    assert(((BencodeString *)blst[1].get())->value == "eggs");
}

void test_bencode_dict() {
    char tok[] = "d3:cow3:moo4:spam4:eggse";
    std::vector<BencodeTokenPtr> lst = parseBencode(tok, strlen(tok));
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_DICT);

    std::map<std::string, BencodeTokenPtr> dict = ((BencodeDictionary *)lst[0].get())->value;
    assert(dict.size() == 2);
    assert( ((BencodeString *)(dict["cow"].get()))->value == "moo" );
    assert( ((BencodeString *)(dict["spam"].get()))->value == "eggs" );
}

void test_bencode_composite() {
    char combined_tok[] = "3:bari:2el:i:3eed:3:cow3:mooe";
}

int main() {
    test_bencode_string();
    test_bencode_integer();
    test_bencode_list();
    test_bencode_dict();
}
