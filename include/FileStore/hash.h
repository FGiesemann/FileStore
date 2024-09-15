/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#ifndef FILESTORE_HASH_H
#define FILESTORE_HASH_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "FileStore/bin_utils.h"
#include "FileStore/file.h"

namespace filestore {

template<size_t num_bits>
struct hash_value {
    static_assert(num_bits > 0 && num_bits % 8 == 0, "only bit lengths divisible by 8 are supported");

    static constexpr auto bitlength = num_bits;
    static constexpr auto bytelength = num_bits / 8;

    std::byte data[bytelength] = {};
};

template<typename HashAlgo>
HashAlgo::hash_type hash_file(const std::filesystem::path &file_path) {
    static constexpr auto buffer_size = 65536U;

    std::ifstream input{file_path, std::ifstream::binary};
    if (!input) {
        throw FileError("Could not open file", file_path);
    }

    HashAlgo algo{};

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);
    while (input) {
        input.read(buffer.get(), buffer_size);
        if (!input.bad())
            algo.update(buffer.get(), input.gcount());
        else
            throw FileError("Error reading input file", file_path);
    }

    return algo.hash();
}

template<size_t num_bits>
std::string to_hex_string(const hash_value<num_bits> &hash) {
    return bytes_to_hex(std::begin(hash.data), std::end(hash.data));
}

} // namespace filestore

#endif
