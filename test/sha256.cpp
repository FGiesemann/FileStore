#include <catch2/catch_test_macros.hpp>

#include "FileStore/hash.h"
#include "FileStore/sha256.h"
#include <filesystem>
#include <string>

std::string hash_str(const std::string &str) {
    filestore::SHA256 sha;
    sha.update(str.data(), str.length());
    return to_hex_string(sha.hash());
}

std::string hash_file(const std::filesystem::path &file_path) {
    const auto hash = filestore::hash_file<filestore::SHA256>(file_path);
    return to_hex_string(hash);
}

TEST_CASE("sha256 algorithm", "[hash]") {
    using namespace std::string_literals;
    std::string str;

    REQUIRE(hash_str("Hello, World!") == "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"s);

    std::string data{
        "adsflbhroliswuh seiguhlsi glgikbsrgoiudfhgsgb<so sjln3408sefs974p94bhjs c jswfos84zsbu hsklusughdsov8vs e iusebixdhlbslisuegp8qw73bv6z 38w g 3wp08w7gfasjuhvas f8 3fvpoa8wzauvfwfa lo87 asg"s};
    REQUIRE(hash_str(data) == "ec4f8460cdc835d41add660236f5059f14e0bd35031df85c6da72bf753ae4efb"s);

    // Data length == SHA256 buffer size (64 bytes)
    data = "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijkl";
    REQUIRE(data.length() == 64);
    REQUIRE(hash_str(data) == "2fcd5a0d60e4c941381fcc4e00a4bf8be422c3ddfafb93c809e8d1e2bfffae8e"s);
}

TEST_CASE("sha256 file hashes", "[hash]") {
    using namespace std::string_literals;
    std::filesystem::path root{"../../test/data"};
    REQUIRE(hash_file(root / "hello.dat") == "dffd6021bb2bd5b0af676290809ec3a53191dd81c7f70a4b28688a362182986f"s);
    REQUIRE(hash_file(root / "file1.dat") == "d5d845d8fd337e1635c929f7205c1bc93ce95bbdd44a23b17b2790c7532d12f1"s);
    REQUIRE(hash_file(root / "file2.dat") == "d5d845d8fd337e1635c929f7205c1bc93ce95bbdd44a23b17b2790c7532d12f1"s);
    REQUIRE(hash_file(root / "file3.dat") == "0cc8d7e70144753c7f1f1ba72687434595934a9dfc0932401fa3285e37eb2b66"s);
    REQUIRE(hash_file(root / "file4.dat") == "d42adb5929b01ea9d435a8d847c71c26a8f7fd8265cb956fdc3210b9cadb2829"s);
}
