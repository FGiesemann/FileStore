/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#include "FileStore/filestore.h"
#include "FileStore/bin_utils.h"
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
        if (!key.increment()) {
            throw FileError("Key space exhausted", file_path);
        }
    }
    const auto path = get_file_path(key);
    fs::create_directories(path.parent_path());
    fs::copy(file_path, path);
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
    Key key{};
    std::memcpy(key.data.data(), hash.data.data(), hash.bytelength);
    return key;
}

bool FileStore::key_exists(const Key &k) const {
    return fs::exists(get_file_path(k));
}

std::string to_string(const Key &k) {
    return bytes_to_hex(std::begin(k.data), std::end(k.data));
}

} // namespace filestore
