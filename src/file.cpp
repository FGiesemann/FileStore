#include "FileStore/file.h"
#include <fstream>

namespace filestore {

bool files_have_same_size(const fs::path &path1, const fs::path &path2) {
    auto size1 = fs::file_size(path1);
    auto size2 = fs::file_size(path2);
    return size1 == size2;
}

bool files_are_equal(const fs::path &existing_file, const fs::path &candidate_file) {
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

} // namespace filestore
