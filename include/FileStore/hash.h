#ifndef FILESTORE_HASH_H
#define FILESTORE_HASH_H

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

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
        throw std::runtime_error("Could not open file");
    }

    HashAlgo algo{};

    std::unique_ptr<char[]> buffer = std::make_unique<char[]>(buffer_size);
    while (input) {
        input.read(buffer.get(), buffer_size);
        if (!input.bad())
            algo.update(buffer.get(), input.gcount());
        else
            throw std::runtime_error("Error reading input file");
    }

    return algo.hash();
}

static inline std::string byte_to_hex(std::byte b) {
    static constexpr auto hexchars = "0123456789abcdef";
    std::string res{2, '0'};
    const auto value = std::to_integer<int>(b);
    res[0] = hexchars[value / 16];
    res[1] = hexchars[value % 16];
    return res;
}

template<size_t num_bits>
std::string to_hex_string(const hash_value<num_bits> &hash) {
    std::stringstream result;
    std::transform(std::begin(hash.data), std::end(hash.data), std::ostream_iterator<std::string>(result), byte_to_hex);
    return result.str();
}

} // namespace filestore

#endif
