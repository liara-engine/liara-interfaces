#include <liara/result.h>
#include <liara/version.h>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int liara_fail(const char* msg) {
    (void)fprintf(stderr, "FAIL: %s\n", msg);
    return EXIT_FAILURE;
}

// NOLINTBEGIN(readability-magic-numbers)
int main(void) {
    uint32_t v = 0xDEADBEEF;

    /* Successful creation */
    v = 0xDEADBEEF;
    liara_result_t res = liara_try_make_version(1U, 2U, 3U, &v);
    if (res != LIARA_RESULT_SUCCESS) { return liara_fail("liara_try_make_version: expected success"); }
    if (LIARA_VERSION_MAJOR(v) != 1U) { return liara_fail("major component mismatch"); }
    if (LIARA_VERSION_MINOR(v) != 2U) { return liara_fail("minor component mismatch"); }
    if (LIARA_VERSION_PATCH(v) != 3U) { return liara_fail("patch component mismatch"); }

    /* Null pointer check */
    res = liara_try_make_version(1U, 2U, 3U, NULL);
    if (res != LIARA_RESULT_NULL_POINTER) { return liara_fail("liara_try_make_version: expected null pointer error"); }

    /* Out of range checks should not modify out_version */
    v = 0xBEEFDEAD;
    res = liara_try_make_version(LIARA_VERSION_MAJOR_MASK + 1U, 0U, 0U, &v);
    if (res != LIARA_RESULT_OUT_OF_RANGE) { return liara_fail("major out-of-range not detected"); }
    if (v != 0xBEEFDEADU) { return liara_fail("out_version modified on major out-of-range"); }

    v = 0xBEEFDEAD;
    res = liara_try_make_version(0U, LIARA_VERSION_MINOR_MASK + 1U, 0U, &v);
    if (res != LIARA_RESULT_OUT_OF_RANGE) { return liara_fail("minor out-of-range not detected"); }
    if (v != 0xBEEFDEADU) { return liara_fail("out_version modified on minor out-of-range"); }

    v = 0xBEEFDEAD;
    res = liara_try_make_version(0U, 0U, LIARA_VERSION_PATCH_MASK + 1U, &v);
    if (res != LIARA_RESULT_OUT_OF_RANGE) { return liara_fail("patch out-of-range not detected"); }
    if (v != 0xBEEFDEADU) { return liara_fail("out_version modified on patch out-of-range"); }

    /* LIARA_MAKE_VERSION_UNSAFE and extraction macros/inline functions */
    uint32_t u = LIARA_MAKE_VERSION_UNSAFE(5U, 6U, 7U);
    if (LIARA_VERSION_MAJOR(u) != 5U) { return liara_fail("LIARA_VERSION_MAJOR macro failed"); }
    if (LIARA_VERSION_MINOR(u) != 6U) { return liara_fail("LIARA_VERSION_MINOR macro failed"); }
    if (LIARA_VERSION_PATCH(u) != 7U) { return liara_fail("LIARA_VERSION_PATCH macro failed"); }

    if (liara_version_major(u) != 5U) { return liara_fail("liara_version_major failed"); }
    if (liara_version_minor(u) != 6U) { return liara_fail("liara_version_minor failed"); }
    if (liara_version_patch(u) != 7U) { return liara_fail("liara_version_patch failed"); }

    /* liara_version_compare behaviour */
    uint32_t a;
    uint32_t b;
    res = liara_try_make_version(1U, 0U, 0U, &a);
    if (res != LIARA_RESULT_SUCCESS) { return liara_fail("setup a failed"); }
    res = liara_try_make_version(1U, 0U, 1U, &b);
    if (res != LIARA_RESULT_SUCCESS) { return liara_fail("setup b failed"); }

    int8_t cmp = liara_version_compare(a, b, false);
    if (!(cmp < 0)) { return liara_fail("compare: expected a < b when considering patch"); }

    cmp = liara_version_compare(a, b, true);
    if (cmp != 0) { return liara_fail("compare: expected equality when ignoring patch"); }

    /* max values */
    res = liara_try_make_version(LIARA_VERSION_MAJOR_MASK, LIARA_VERSION_MINOR_MASK, LIARA_VERSION_PATCH_MASK, &v);
    if (res != LIARA_RESULT_SUCCESS) { return liara_fail("creating max version failed"); }
    if (LIARA_VERSION_MAJOR(v) != LIARA_VERSION_MAJOR_MASK) { return liara_fail("max major mismatch"); }
    if (LIARA_VERSION_MINOR(v) != LIARA_VERSION_MINOR_MASK) { return liara_fail("max minor mismatch"); }
    if (LIARA_VERSION_PATCH(v) != LIARA_VERSION_PATCH_MASK) { return liara_fail("max patch mismatch"); }

    puts("OK");
    return EXIT_SUCCESS;
}

// NOLINTEND(readability-magic-numbers)
