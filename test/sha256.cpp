#include <catch2/catch_test_macros.hpp>

#include "FileStore/sha256.h"
#include <string>

std::string hash(const std::string &str) {
    filestore::SHA256 sha;
    sha.update(str.data(), str.length());
    return to_hex_string(sha.hash());
}

TEST_CASE("sha256 algorithm", "[hash]") {
    using namespace std::string_literals;
    std::string str;

    REQUIRE(hash("Hello, World!") == "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"s);

    std::string data{
        "adsflbhroliswuh seiguhlsi glgikbsrgoiudfhgsgb<so sjln3408sefs974p94bhjs c jswfos84zsbu hsklusughdsov8vs e iusebixdhlbslisuegp8qw73bv6z 38w g 3wp08w7gfasjuhvas f8 3fvpoa8wzauvfwfa lo87 asg"};
    REQUIRE(hash(data) == "ec4f8460cdc835d41add660236f5059f14e0bd35031df85c6da72bf753ae4efb");

    // Data length == SHA256 buffer size (64 bytes)
    data = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijkl";
    REQUIRE(data.length() == 64);
    REQUIRE(hash(data) == "2fcd5a0d60e4c941381fcc4e00a4bf8be422c3ddfafb93c809e8d1e2bfffae8e");
}
