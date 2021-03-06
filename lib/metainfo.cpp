#include "metainfo.h"

/* MetaInfo definitions */
MetaInfo::MetaInfo(std::string path) {
    using namespace std;

    // parse bencode tokens from file
    ifstream metainfo_file;
    metainfo_file.open(path.c_str(), ios::out | ios::binary);
    if(!metainfo_file.is_open()) {
        this->mValid = false;
        return;
    }

    metainfo_file.seekg(0, ios::end);
    // we don't want to EOF character
    int file_size = (int)metainfo_file.tellg() - 1;
    metainfo_file.seekg(0, ios::beg);


    char contents[file_size];
    metainfo_file.read(contents, file_size);
    metainfo_file.close();

    vector<BencodeTokenPtr> toks = BencodeToken::parseBencode(contents, file_size);
    if(toks.size() != 1 || toks[0]->type != BE_DICT) {
        this->mValid = false;
        return;
    }

    // make sure file_dict has the keys we expect
    map<string, BencodeTokenPtr> file_dict = BencodeDictionary::get_value(toks[0]);
    if(file_dict.find("info") == file_dict.end() || file_dict.find("announce") == file_dict.end()) {
        this->mValid = false;
        return;
    }

    // extract values from file_dict
    this->tracker_url = BencodeString::get_value(file_dict["announce"]);
    map<string, BencodeTokenPtr> info_dict = BencodeDictionary::get_value(file_dict["info"]);

    // add values from info_dict to the proper member variables
    this->piece_length = BencodeInteger::get_value(info_dict["piece length"]);
    this->pieces = PieceInfo::parse(
        BencodeString::get_value(info_dict["pieces"])
    );

    if(info_dict.find("files") == info_dict.end()) {
        // single file mode
        this->files = FileInfo::parse_single(
            BencodeString::get_value(info_dict["name"]),
            BencodeInteger::get_value(info_dict["length"])
        );
    } else {
        // multi file mode
        this->dir_name = BencodeString::get_value(info_dict["name"]);
        this->files = FileInfo::parse_multiple(
            BencodeList::get_value(info_dict["files"])
        );
    }

    this->mValid = true;
}

bool MetaInfo::valid() {
    return this->mValid;
}


/* PieceInfo definitions */
PieceInfo::PieceInfo(const char *hash_str, int piece_id) : piece_id(piece_id) {
    memcpy(this->sha1_hash, hash_str, SHA1_LEN);
}

std::vector<PieceInfo> PieceInfo::parse(std::string hash_str) {
    const char * hash_cstr = hash_str.c_str();
    size_t len = hash_str.size();
    std::vector<PieceInfo> piece_infos;
    int piece_num = 0;
    for(int i=0; i<len; i+= SHA1_LEN) {
        piece_infos.push_back(PieceInfo(hash_cstr + i, piece_num));
        piece_num++;
    }
    assert(piece_infos.size() == piece_num);
    return piece_infos;
}



/* FileInfo definitions */
FileInfo::FileInfo(std::string name, int length) : name(name), length(length) {}


std::vector<FileInfo> FileInfo::parse_single(std::string name, int length) {
    std::vector<FileInfo> out;
    out.push_back(FileInfo(name, length));
    return out;
}

std::vector<FileInfo> FileInfo::parse_multiple(std::vector<BencodeTokenPtr> file_dicts) {
    std::vector<FileInfo> out;
    for(int i=0; i<file_dicts.size(); i++) {
         std::map<std::string, BencodeTokenPtr> file_dict = BencodeDictionary::get_value(file_dicts[i]);
         int length = BencodeInteger::get_value(file_dict["length"]);
         std::vector<BencodeTokenPtr> path_list = BencodeList::get_value(file_dict["path"]);
         std::string name = "";
         for(int j=0; j<path_list.size(); j++) {
             name.append(BencodeString::get_value(path_list[i]));
             name.append("/");
         }
        out.push_back(FileInfo(name, length));
    }
    return out;
}
