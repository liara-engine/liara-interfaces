#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <liara/version.h>

TEST_CASE("Version encoding and decoding") {
    uint32_t version = LIARA_MAKE_VERSION(1, 2, 3);
    CHECK(LIARA_VERSION_MAJOR(version) == 1);
    CHECK(LIARA_VERSION_MINOR(version) == 2);
    CHECK(LIARA_VERSION_PATCH(version) == 3);

    uint32_t interface_version = LIARA_INTERFACE_VERSION;
    CHECK(LIARA_VERSION_MAJOR(interface_version) == 0);
    CHECK(LIARA_VERSION_MINOR(interface_version) == 0);
    CHECK(LIARA_VERSION_PATCH(interface_version) == 0);
}
