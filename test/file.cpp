/* ******************************************************* *
 * FileStore                                               *
 * (c) Florian Giesemann 2024                              *
 * ******************************************************* */

#include <catch2/catch_test_macros.hpp>

#include "FileStore/file.h"

TEST_CASE("file size comparison", "[file]") {
    using namespace filestore;
    std::filesystem::path root{"../../test/data"};

    REQUIRE(files_have_same_size(root / "file1.dat", root / "file2.dat"));
    REQUIRE_FALSE(files_have_same_size(root / "file1.dat", root / "file3.dat"));
    REQUIRE(files_have_same_size(root / "file3.dat", root / "file4.dat"));
}

TEST_CASE("file equality checks", "[file]") {
    using namespace filestore;
    std::filesystem::path root{"../../test/data"};

    REQUIRE(files_are_equal(root / "file1.dat", root / "file2.dat"));
    REQUIRE_FALSE(files_are_equal(root / "file1.dat", root / "file3.dat"));
    REQUIRE_FALSE(files_are_equal(root / "file3.dat", root / "file4.dat"));
}
