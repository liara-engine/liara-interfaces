#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <liara/version.h>

TEST_CASE("version_cxx") {
    uint32_t version = LIARA_MAKE_VERSION(1, 2, 3);
    CHECK(LIARA_VERSION_MAJOR(version) == 1);
    CHECK(LIARA_VERSION_MINOR(version) == 2);
    CHECK(LIARA_VERSION_PATCH(version) == 3);
}
