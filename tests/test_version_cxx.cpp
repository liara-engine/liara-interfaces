// NOLINTBEGIN(readability-identifier-naming)
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
// NOLINTEND(readability-identifier-naming)

#include <liara/result.h>
#include <liara/version.h>

#include <cstdint>

#include <doctest/doctest.h>

TEST_CASE("liara_try_make_version: success and extraction") {
    uint32_t v = 0U;
    CHECK(liara_try_make_version(1U, 2U, 3U, &v) == LIARA_RESULT_SUCCESS);
    CHECK(LIARA_VERSION_MAJOR(v) == 1U);
    CHECK(LIARA_VERSION_MINOR(v) == 2U);
    CHECK(LIARA_VERSION_PATCH(v) == 3U);

    CHECK(liara_version_major(v) == 1U);
    CHECK(liara_version_minor(v) == 2U);
    CHECK(liara_version_patch(v) == 3U);
}

TEST_CASE("liara_try_make_version: null pointer and out of range") {
    uint32_t sentinel = 0xC0FFEEU;
    uint32_t v = sentinel;

    CHECK(liara_try_make_version(1U, 2U, 3U, nullptr) == LIARA_RESULT_NULL_POINTER);

    v = sentinel;
    CHECK(liara_try_make_version(LIARA_VERSION_MAJOR_MASK + 1U, 0U, 0U, &v) == LIARA_RESULT_OUT_OF_RANGE);
    CHECK(v == sentinel);

    v = sentinel;
    CHECK(liara_try_make_version(0U, LIARA_VERSION_MINOR_MASK + 1U, 0U, &v) == LIARA_RESULT_OUT_OF_RANGE);
    CHECK(v == sentinel);

    v = sentinel;
    CHECK(liara_try_make_version(0U, 0U, LIARA_VERSION_PATCH_MASK + 1U, &v) == LIARA_RESULT_OUT_OF_RANGE);
    CHECK(v == sentinel);
}

TEST_CASE("unsafe macro and masks") {
    auto u = LIARA_MAKE_VERSION_UNSAFE(5U, 6U, 7U);
    CHECK(LIARA_VERSION_MAJOR(u) == 5U);
    CHECK(LIARA_VERSION_MINOR(u) == 6U);
    CHECK(LIARA_VERSION_PATCH(u) == 7U);
}

TEST_CASE("liara_version_compare behaviour") {
    uint32_t a;
    uint32_t b;
    CHECK(liara_try_make_version(1U, 0U, 0U, &a) == LIARA_RESULT_SUCCESS);
    CHECK(liara_try_make_version(1U, 0U, 1U, &b) == LIARA_RESULT_SUCCESS);

    CHECK(liara_version_compare(a, b, false) < 0);
    CHECK(liara_version_compare(a, b, true) == 0);  // ignoring patch should be equal

    uint32_t x;
    uint32_t y;
    CHECK(liara_try_make_version(2U, 0U, 0U, &x) == LIARA_RESULT_SUCCESS);
    CHECK(liara_try_make_version(1U, 999U, 999U, &y) == LIARA_RESULT_SUCCESS);
    CHECK(liara_version_compare(x, y, false) > 0);
}

TEST_CASE("max values roundtrip") {
    uint32_t v;
    CHECK(liara_try_make_version(LIARA_VERSION_MAJOR_MASK, LIARA_VERSION_MINOR_MASK, LIARA_VERSION_PATCH_MASK, &v)
          == LIARA_RESULT_SUCCESS);
    CHECK(LIARA_VERSION_MAJOR(v) == LIARA_VERSION_MAJOR_MASK);
    CHECK(LIARA_VERSION_MINOR(v) == LIARA_VERSION_MINOR_MASK);
    CHECK(LIARA_VERSION_PATCH(v) == LIARA_VERSION_PATCH_MASK);
}
