/*Tests bencoding function works correctly*/

#include <cassert>
#include <iostream>
#include "metainfo.h"

#define TORRENT_FILE "tests/test.torrent"

int main() {
    std::string path(TORRENT_FILE);
    MetaInfo metainfo(path);

    assert(metainfo.tracker_url == "http://torrent.ubuntu.com:6969/announce");
    assert(metainfo.piece_length == 524288);

    assert(metainfo.files.size() == 1);
    assert(metainfo.files[0].name == "ubuntu-12.10-desktop-amd64.iso");
    assert(metainfo.files[0].length == 800063488);
}
