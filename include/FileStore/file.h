/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#ifndef FILESTORE_FILE_H
#define FILESTORE_FILE_H

#include <filesystem>
#include <string>

namespace filestore {

namespace fs = std::filesystem;

class FileError : public std::runtime_error {
public:
    FileError(const std::string &msg, const fs::path &path) : std::runtime_error(msg), path{path} {}

    const fs::path &filePath() const { return path; }
private:
    fs::path path;
};

bool files_have_same_size(const fs::path &path1, const fs::path &path2);
bool files_are_equal(const fs::path &existing_file, const fs::path &candidate_path);

} // namespace filestore

#endif
