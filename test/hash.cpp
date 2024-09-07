#include <catch2/catch_test_macros.hpp>

#include "FileStore/hash.h"
#include <string>

TEST_CASE("hash to string conversion", "[hash]") {
    using namespace filestore;
    using namespace std::string_literals;
    std::string str;

    hash_value<8> h1{std::byte{12}};
    str = to_hex_string(h1);
    REQUIRE(str.length() == 2);
    REQUIRE(str == "0c"s);

    hash_value<16> h2{std::byte{214}, std::byte{167}};
    str = to_hex_string(h2);
    REQUIRE(str.length() == 4);
    REQUIRE(str == "d6a7"s);

    hash_value<256> h3;
    str = to_hex_string(h3);
    REQUIRE(str.length() == 64);
    REQUIRE(str == "0000000000000000000000000000000000000000000000000000000000000000"s);

    const int bytes[32] = {34, 237, 137, 2, 65, 24, 235, 123, 52, 252, 163, 62, 156, 26, 27, 146, 172, 36, 27, 219, 74, 53, 0, 252, 250, 178, 46, 24, 0, 0, 34, 173};
    std::transform(std::begin(bytes), std::end(bytes), std::begin(h3.data), [](int i) { return std::byte(i); });
    str = to_hex_string(h3);
    REQUIRE(str.length() == 64);
    REQUIRE(str == "22ed89024118eb7b34fca33e9c1a1b92ac241bdb4a3500fcfab22e18000022ad"s);
}
