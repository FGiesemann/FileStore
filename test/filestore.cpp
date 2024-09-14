/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#include <catch2/catch_test_macros.hpp>

#include "FileStore/filestore.h"
#include <filesystem>
#include <random>

std::string rand_string(size_t length) {
    static constexpr auto num_chars = 63U;
    static constexpr char chars[num_chars] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

    static constexpr auto rand_char = [&]() {
        static std::default_random_engine rng(std::random_device{}());
        static std::uniform_int_distribution<> dist(0, num_chars);
        return chars[dist(rng)];
    };

    std::string str(length, 0);
    std::generate_n(std::begin(str), length, rand_char);

    return str;
}

class TempFS {
public:
    TempFS() {
        m_path = std::filesystem::temp_directory_path() / rand_string(5);
        while (std::filesystem::exists(m_path)) {
            m_path = std::filesystem::temp_directory_path() / rand_string(5);
        }
    }
    ~TempFS() { std::filesystem::remove_all(m_path); }

    const std::filesystem::path &path() const { return m_path; }
    operator std::filesystem::path &() { return m_path; }
private:
    std::filesystem::path m_path;
};

filestore::Key gen_key(const std::string &hash, int distinguisher) {
    filestore::Key key{};
    for (int i = 0; i < key.hash.bytelength; ++i) {
        key.hash.data[i] = static_cast<std::byte>(std::stoi(hash.substr(2 * i, 2), nullptr, 16));
    }
    key.distinguisher = distinguisher;
    return key;
}

TEST_CASE("FileStore key", "[filestore]") {
    using namespace filestore;

    const auto k1 = gen_key("a37b2f29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f2872", 0);
    REQUIRE(to_string(k1) == "a37b2f29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f287200000000");
    const auto k2 = gen_key("987e3458ef4567623f54aab8765a63c0a78e62e3a54976d52ee358c76e543fef", 6365);
    REQUIRE(to_string(k2) == "987e3458ef4567623f54aab8765a63c0a78e62e3a54976d52ee358c76e543fef000018dd");
}

TEST_CASE("FileStore key generation", "[filestore]") {
    using namespace filestore;

    std::filesystem::path root{"../../test/data"};

    const auto k1 = generate_file_key(root / "file1.dat");
    REQUIRE(to_string(k1) == "d5d845d8fd337e1635c929f7205c1bc93ce95bbdd44a23b17b2790c7532d12f100000000");
    const auto k2 = generate_file_key(root / "file3.dat");
    REQUIRE(to_string(k2) == "0cc8d7e70144753c7f1f1ba72687434595934a9dfc0932401fa3285e37eb2b6600000000");
}

TEST_CASE("FileStore key increment", "[filestore]") {
    using namespace filestore;

    Key k1{};
    REQUIRE(k1.distinguisher == 0);
    increment_key(k1);
    REQUIRE(k1.distinguisher == 1);
}

TEST_CASE("FileStore file paths", "[filestore]") {
    using namespace filestore;
    namespace fs = std::filesystem;

    const auto k1 = gen_key("a37b2f29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f2872", 0);
    const auto k2 = gen_key("a37b2f29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f2872", 6365);

    TempFS fs1;
    FileStore store1(fs1);
    REQUIRE(store1.get_file_path(k1) == fs1.path() / "a3" / "7b" / "2f29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f287200000000");
    REQUIRE(store1.get_file_path(k2) == fs1.path() / "a3" / "7b" / "2f29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f2872000018dd");

    TempFS fs2;
    FileStore store2(fs2, 3);
    REQUIRE(store2.get_file_path(k1) == fs2.path() / "a3" / "7b" / "2f" / "29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f287200000000");
    REQUIRE(store2.get_file_path(k2) == fs2.path() / "a3" / "7b" / "2f" / "29ec7ef15a39b20015382bef6a45b28a3b2ffe1862ca4f92ef7a3f2872000018dd");
}

TEST_CASE("FileStore import test", "[filestore]") {
    using namespace filestore;
    namespace fs = std::filesystem;
    fs::path root{"../../test/data"};

    TempFS fs1;
    FileStore store(fs1);
    const auto k1 = store.import(root / "file1.dat");
    REQUIRE(k1.has_value());
    REQUIRE(to_string(k1.value()) == "d5d845d8fd337e1635c929f7205c1bc93ce95bbdd44a23b17b2790c7532d12f100000000");
    REQUIRE(fs::exists(store.get_file_path(k1.value())));

    const auto k2 = store.import(root / "file2.dat");
    REQUIRE_FALSE(k2.has_value());
    REQUIRE(k1.value() == k2.error());

    const auto k3 = store.import(root / "file3.dat");
    REQUIRE(k3.has_value());
    REQUIRE(to_string(k3.value()) == "0cc8d7e70144753c7f1f1ba72687434595934a9dfc0932401fa3285e37eb2b6600000000");
    REQUIRE(fs::exists(store.get_file_path(k3.value())));
}
