/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#ifndef FILESTORE_FILESTORE_H
#define FILESTORE_FILESTORE_H

#include "FileStore/sha256.h"
#include <expected>
#include <filesystem>

namespace filestore {

namespace fs = std::filesystem;

struct Key {
    SHA256::hash_type hash;
    uint32_t distinguisher;
};

inline bool operator==(const Key &k1, const Key &k2) {
    return k1.distinguisher == k2.distinguisher && std::equal(std::begin(k1.hash.data), std::end(k1.hash.data), std::begin(k2.hash.data));
}

std::string to_string(const Key &k);

class FileStore {
public:
    using import_result = std::expected<Key, Key>;

    explicit FileStore(const fs::path &root_path, int folder_levels = 2);

    import_result import(const fs::path &file_path);
    fs::path get_file_path(const Key &file_key) const;

    const fs::path &root_path() const { return m_root_path; }
private:
    fs::path m_root_path;
    int m_folder_levels{2};

    bool key_exists(const Key &k) const;
};

Key generate_file_key(const fs::path &file_path);
void increment_key(Key &k);

} // namespace filestore

#endif
