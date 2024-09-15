/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#ifndef FILESTORE_HASH_H
#define FILESTORE_HASH_H

#include <algorithm>
#include <array>
#include <filesystem>
#include <fstream>
#include <span>
#include <sstream>
#include <string>
#include <vector>

#include "FileStore/bin_utils.h"
#include "FileStore/file.h"

namespace filestore {

template<size_t num_bits>
struct hash_value {
    static_assert(num_bits > 0 && num_bits % 8 == 0, "only bit lengths divisible by 8 are supported");

    static constexpr auto bitlength = num_bits;
    static constexpr auto bytelength = num_bits / 8;

    std::array<std::byte, bytelength> data{};
};

template<typename HashAlgo>
HashAlgo::hash_type hash_file(const std::filesystem::path &file_path) {
    static constexpr auto buffer_size = 65536U;

    std::ifstream input{file_path, std::ifstream::binary};
    if (!input) {
        throw FileError("Could not open file", file_path);
    }

    HashAlgo algo{};

    std::vector<char> buffer(buffer_size);
    while (input) {
        input.read(buffer.data(), buffer_size);
        if (!input.bad())
            algo.update(std::span(buffer.data(), input.gcount()));
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
