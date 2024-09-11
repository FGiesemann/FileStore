#include "FileStore/filestore.h"
#include "FileStore/file.h"
#include "FileStore/hash.h"

namespace filestore {

FileStore::import_result FileStore::import(const fs::path &file_path) {
    auto key = generate_file_key(file_path);
    while (key_exists(key)) {
        if (files_are_equal(get_file_path(key), file_path))
            return std::unexpected(key);
        increment_key(key);
    }
    return key;
}

fs::path FileStore::get_file_path(const Key &file_key) const {
    const auto file_name = to_string(file_key);
    return root_path() / file_name.substr(0, 2) / file_name.substr(2, 2) / file_name.substr(4);
}

Key generate_file_key(const fs::path &file_path) {
    const auto hash = hash_file<SHA256>(file_path);
    return Key{hash, 0U};
}

bool FileStore::key_exists(const Key &k) const {
    return fs::exists(get_file_path(k));
}

void increment_key(Key &k) {
    if (k.distinguisher < std::numeric_limits<uint32_t>::max()) {
        ++k.distinguisher;
    }
}

std::string to_string(const Key &k) {
    std::stringstream str;
    str << to_hex_string(k.hash);
    str << std::hex << std::setfill('0') << std::setw(8) << k.distinguisher;
    return str.str();
}

} // namespace filestore
