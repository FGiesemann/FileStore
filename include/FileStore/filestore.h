#ifndef FILESTORE_FILESTORE_H
#define FILESTORE_FILESTORE_H

#include <expected>
#include <filesystem>

namespace filestore {

namespace fs = std::filesystem;

struct Key {};

class FileStore {
public:
    using import_result = std::expected<Key, Key>;

    FileStore(const fs::path &root_path) : m_root_path{root_path} {}

    import_result import(const fs::path &file_path);
    fs::path get_file_path(const Key &file_key) const;

    const fs::path &root_path() const { return m_root_path; }
private:
    fs::path m_root_path;
};

} // namespace filestore

#endif
