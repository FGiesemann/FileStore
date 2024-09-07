#include "FileStore/filestore.h"
#include "FileStore/hash.h"

namespace filestore {

FileStore::import_result FileStore::import(const fs::path &file_path) {
    auto key = generate_file_key(file_path);
    while (key_exists(key)) {
        if (is_duplicate_file(get_file_path(key), file_path))
            return std::unexpected(key);
        increment_key(key);
    }
    return key;
}

fs::path FileStore::get_file_path(const Key &file_key) const {
    const auto file_name = to_string(file_key);
    return root_path() / file_name.substr(0, 2) / file_name.substr(2);
}

Key FileStore::generate_file_key(const fs::path &file_path) {
    const auto hash = hash_file<SHA256>(file_path);
    return Key{hash, 0U};
}

bool FileStore::key_exists(const Key &k) const {
    return fs::exists(get_file_path(k));
}

bool FileStore::is_duplicate_file(const fs::path &existing_file, const fs::path &candidate_file) {
    constexpr std::streamsize BufferSize = 65536u;

    if (!files_have_same_size(existing_file, candidate_file))
        return false;

    std::ifstream file1(existing_file, std::ios_base::binary);
    std::ifstream file2(candidate_file, std::ios_base::binary);

    if (!file1.is_open())
        throw FileError{"Could not open file", existing_file};
    if (!file2.is_open())
        throw FileError{"Could not open file", candidate_file};

    std::unique_ptr<char[]> buffer1 = std::make_unique<char[]>(BufferSize);
    std::unique_ptr<char[]> buffer2 = std::make_unique<char[]>(BufferSize);
    file1.read(buffer1.get(), BufferSize);
    file2.read(buffer2.get(), BufferSize);
    while (file1 && file2) {
        if (std::memcmp(buffer1.get(), buffer2.get(), file1.gcount()) != 0) {
            return false;
        }
        file1.read(buffer1.get(), BufferSize);
        file2.read(buffer2.get(), BufferSize);
    }
    if (file1.bad())
        throw FileError{"Error reading input file", existing_file};
    if (file2.bad())
        throw FileError{"Error reading input file", candidate_file};

    return true;
}

bool FileStore::files_have_same_size(const fs::path &path1, const fs::path &path2) {
    auto size1 = fs::file_size(path1);
    auto size2 = fs::file_size(path2);
    return size1 == size2;
}

void FileStore::increment_key(Key &k) {
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
