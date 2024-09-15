/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#ifndef FILESTORE_BIN_UTILS_H
#define FILESTORE_BIN_UTILS_H

#include <string>

namespace filestore {

inline std::string byte_to_hex(std::byte b) {
    static constexpr auto hexchars = "0123456789abcdef";
    std::string res{2, '0'};
    const auto value = std::to_integer<int>(b);
    res[0] = hexchars[value / 16];
    res[1] = hexchars[value % 16];
    return res;
}

template<typename I>
inline std::string bytes_to_hex(I begin, I end) {
    std::string res;
    const auto length = std::distance(begin, end);
    res.reserve(length * 2);
    I it{begin};
    while (it != end)
        res += byte_to_hex(*it++);
    return res;
}

} // namespace filestore

#endif
