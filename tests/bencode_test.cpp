#include <cassert>
#include <cstring>
#include <vector>
#include "bencode.h"

int main() {
    char str_tok[] = "3:foo";
    char int_tok[] = "i:200e";
    char list_tok[] = "l4:spam4:eggse";
    char dict_tok[] = "d3:cow3:moo4:spam4:eggse";
    char combined_tok[] = "3:bari:2el:i:3eed:3:cow3:mooe";

    // TODO: fix object slicing problem
    std::vector<BencodeTokenPtr> lst = parseBencode(str_tok, strlen(str_tok)+1);
    assert(lst.size() == 1);
    assert(lst[0]->type == BE_STRING);
    assert(
        ((BencodeString *)lst[0].get())->value == "foo"
    );
}
