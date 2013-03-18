/*Tests bencoding function works correctly*/

#include <cassert>
#include <cstring>
#include <vector>
#include <iostream>
#include "bencode.h"

void test_bencode_string() {
    char tok[] = "3:foo";
    std::vector<BencodeTokenPtr> lst = BencodeToken::parseBencode(tok, strlen(tok));
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_STRING);

    std::string str_val = ((BencodeString *)lst[0].get())->value;
    assert(str_val == "foo");
}

void test_bencode_integer() {
    char tok[] = "i200e";
    std::vector<BencodeTokenPtr> lst = BencodeToken::parseBencode(tok, strlen(tok));

    assert(lst.size() == 1);
    assert(lst[0]->type == BE_INTEGER);

    int val = ((BencodeInteger *)lst[0].get())->value;
    assert(val == 200);
}

void test_bencode_list() {
    char tok[] = "l4:spam4:eggse";
    std::vector<BencodeTokenPtr> lst = BencodeToken::parseBencode(tok, strlen(tok));
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_LIST);

    std::vector<BencodeTokenPtr> blst = ((BencodeList *)lst[0].get())->value;
    //std::cout << "foo " << blst.size() << std::endl;
    assert(blst.size() == 2);
    assert(((BencodeString *)blst[0].get())->value == "spam");
    assert(((BencodeString *)blst[1].get())->value == "eggs");
}

void test_bencode_dict() {
    char tok[] = "d3:cow3:moo4:spam4:eggse";
    std::vector<BencodeTokenPtr> lst = BencodeToken::parseBencode(tok, strlen(tok));
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_DICT);

    std::map<std::string, BencodeTokenPtr> dict = ((BencodeDictionary *)lst[0].get())->value;
    assert(dict.size() == 2);
    assert( ((BencodeString *)(dict["cow"].get()))->value == "moo" );
    assert( ((BencodeString *)(dict["spam"].get()))->value == "eggs" );
}

void test_bencode_composite() {
    char tok[] = "3:bari2eli3eed3:cow3:mooe";

    // we expect [<BE_STRING>, <BE_INTEGER>, <BE_LIST>, <BE_DICT>]
    std::vector<BencodeTokenPtr> lst = BencodeToken::parseBencode(tok, strlen(tok));
    //std::cout << "foo " << lst.size() << std::endl;
    assert(lst.size() == 4);
    assert(lst[0]->type == BE_STRING);
    assert(lst[1]->type == BE_INTEGER);
    assert(lst[2]->type == BE_LIST);
    assert(lst[3]->type == BE_DICT);

    assert(BencodeString::get_value(lst[0]) == "bar");
    assert(BencodeInteger::get_value(lst[1]) == 2);

    std::vector<BencodeTokenPtr> blst = BencodeList::get_value(lst[2]);
    assert(blst.size() == 1);
    assert(BencodeInteger::get_value(blst[0]) == 3);
}

int main() {
    test_bencode_string();
    test_bencode_integer();
    test_bencode_list();
    test_bencode_dict();
    test_bencode_composite();
}
