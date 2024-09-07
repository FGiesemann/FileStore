#include "FileStore/filestore.h"

#include "FileStore/hash.h"
#include "FileStore/sha256.h"

namespace filestore {

FileStore::import_result FileStore::import(const fs::path &file_path) {
    const auto hash = hash_file<SHA256>(file_path);
    // const auto stored_files = enumerate_files_with_hash(hash);
    // for (const auto &f : stored_files) {
    //     if (files_equal(file_path, f.path())) {
    //         return std::unexpect(f.key());
    //     }
    // }

    // const auto key = generate_file_key(hash);
    // store_file(file_path, key);
    return Key{};
}

fs::path FileStore::get_file_path(const Key &file_key) const {
    return {};
}

} // namespace filestore
