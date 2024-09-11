#include "FileStore/filestore.h"
#include "FileStore/file.h"
#include "FileStore/hash.h"

namespace filestore {

FileStore::FileStore(const fs::path &root, int folder_levels) : m_root_path{root}, m_folder_levels{folder_levels} {
    // no need to check, if m_root_path exists. In that case, the call does nothing
    fs::create_directories(m_root_path);
}

FileStore::import_result FileStore::import(const fs::path &file_path) {
    auto key = generate_file_key(file_path);
    while (key_exists(key)) {
        if (files_are_equal(get_file_path(key), file_path))
            return std::unexpected(key);
        increment_key(key);
    }
    // TODO: store the file!
    return key;
}

fs::path FileStore::get_file_path(const Key &file_key) const {
    const auto file_name = to_string(file_key);
    fs::path file_path = root_path();
    for (int i = 0; i < m_folder_levels; ++i)
        file_path /= file_name.substr(2 * i, 2);
    return file_path / file_name.substr(m_folder_levels * 2);
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
