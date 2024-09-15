/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#ifndef FILESTORE_FILESTORE_H
#define FILESTORE_FILESTORE_H

#include "FileStore/sha256.h"
#include <array>
#include <expected>
#include <filesystem>

namespace filestore {

namespace fs = std::filesystem;

struct Key {
    using distinguisher_type = std::uint32_t;
    static constexpr auto hash_size = 32U;
    static constexpr auto distinguisher_size = sizeof(distinguisher_type);
    static constexpr auto bytelength = hash_size + distinguisher_size;

    std::array<std::byte, bytelength> data;
};

inline bool operator==(const Key &k1, const Key &k2) {
    return k1.data == k2.data;
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
uint32_t extract_distinguisher(const Key &k);
void update_distinguisher(Key &k, uint32_t d);
void increment_key(Key &k);

} // namespace filestore

#endif
