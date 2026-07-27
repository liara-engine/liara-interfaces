// NOLINTBEGIN(readability-identifier-naming)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// NOLINTEND(readability-identifier-naming)
#include <liara/abi_version.h>
#include <liara/result.h>
#include <liara/version.h>

#include <cstdint>

#include <doctest/doctest.h>

TEST_CASE("Version encoding and decoding") {
    uint32_t version;
    REQUIRE(liara_try_make_version(1, 2, 3, &version) == LIARA_RESULT_SUCCESS);
    CHECK(LIARA_VERSION_MAJOR(version) == 1);
    CHECK(LIARA_VERSION_MINOR(version) == 2);
    CHECK(LIARA_VERSION_PATCH(version) == 3);

    auto interface_version = LIARA_ABI_VERSION;
    CHECK(LIARA_VERSION_MAJOR(interface_version) == 0);
    CHECK(LIARA_VERSION_MINOR(interface_version) == 1);
    CHECK(LIARA_VERSION_PATCH(interface_version) == 0);
}
