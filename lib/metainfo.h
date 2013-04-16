#ifndef METAINFO_H
#define METAINFO_H

#include <map>
#include <string>
#include <vector>
#include <fstream>

#include "common.h"
#include "bencode.h"

/* Stores the info for a piece */
class PieceInfo {
public:
    PieceInfo(const char *sha1_hash, int piece_id);
    int piece_id;           // order of the piece
    char sha1_hash[SHA1_LEN];  // the SHA1 hash for this piece

    // parses byte string concatenation of all 20-byte SHA1 hash values for
    // pieces to a list of PieceInfo objects
    static std::vector<PieceInfo> parse(std::string hash_str);
};

/* Stores the info for a file */
class FileInfo {
public:
    FileInfo(std::string name, int length);
    std::string name;   // name of file
    int length;         // size of file in bytes
    std::string path;   // path of file if in multifile mode

    // returns a list of FileInfo objects based on input from metainfo file
    static std::vector<FileInfo> parse_single(std::string name, int length);
    static std::vector<FileInfo> parse_multiple(std::vector<BencodeTokenPtr> file_dicts);
};

/* Information from a metainfo file */
class MetaInfo {
public:
    MetaInfo(std::string path);


    // returns whether or not metainfo file was successfully parsed
    bool valid();

    // number of bytes in each piece
    int piece_length;

    // SHA1 hash of the info for the tracker
    char info_hash[SHA1_LEN]; // TODO

    // hashes for each piece
    std::vector<PieceInfo> pieces;

    // name of top-level directory if multifile mode
    // empty string if single user mode
    std::string dir_name;

    // list of files
    std::vector<FileInfo> files;

    // url for tracker
    // TODO: extract port from url
    std::string tracker_url;

private:
    bool mValid;
};

#endif
