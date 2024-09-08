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

std::string to_string(const Key &k);

class FileError : public std::runtime_error {
public:
    FileError(const std::string &msg, const fs::path &path) : std::runtime_error(msg), path{path} {}

    const fs::path &filePath() const { return path; }
private:
    fs::path path;
};

class FileStore {
public:
    using import_result = std::expected<Key, Key>;

    FileStore(const fs::path &root_path) : m_root_path{root_path} {}

    import_result import(const fs::path &file_path);
    fs::path get_file_path(const Key &file_key) const;

    const fs::path &root_path() const { return m_root_path; }
private:
    fs::path m_root_path;

    bool key_exists(const Key &k) const;
};

Key generate_file_key(const fs::path &file_path);
bool is_duplicate_file(const fs::path &existing_file, const fs::path &candidate_path);
bool files_have_same_size(const fs::path &path1, const fs::path &path2);
void increment_key(Key &k);

} // namespace filestore

#endif
