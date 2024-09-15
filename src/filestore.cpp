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
        increment_key(key);
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
    std::memcpy(key.data.data(), hash.data, hash.bytelength);
    return key;
}

bool FileStore::key_exists(const Key &k) const {
    return fs::exists(get_file_path(k));
}

uint32_t extract_distinguisher(const Key &k) {
    Key::distinguisher_type d;
    std::memcpy(&d, k.data.data() + k.hash_size, k.distinguisher_size);
    return d;
}

void update_distinguisher(Key &k, Key::distinguisher_type d) {
    std::memcpy(k.data.data() + k.hash_size, &d, k.distinguisher_size);
}

void increment_key(Key &k) {
    auto d = extract_distinguisher(k);
    if (d < std::numeric_limits<Key::distinguisher_type>::max()) {
        update_distinguisher(k, d + 1);
    }
}

std::string to_string(const Key &k) {
    return bytes_to_hex(std::begin(k.data), std::end(k.data));
}

} // namespace filestore
