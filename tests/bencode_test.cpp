/*Tests bencoding function works correctly*/

#include <cassert>
#include <cstring>
#include <vector>
#include "bencode.h"

void test_bencode_string() {
    char str_tok[] = "3:foo";
    std::vector<BencodeTokenPtr> lst = parseBencode(str_tok, strlen(str_tok)+1);
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_STRING);
    assert(
        ((BencodeString *)lst[0].get())->value == "foo"
    );
}

void test_bencode_integer() {
    char int_tok[] = "i:200e";

}

void test_bencode_list() {
    char list_tok[] = "l4:spam4:eggse";
}

void test_bencode_dict() {
    char dict_tok[] = "d3:cow3:moo4:spam4:eggse";
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
