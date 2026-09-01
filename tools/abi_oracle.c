/**
 * @file abi_oracle.c
 * @brief Exposes `liara_version_provides` to the documentation pipeline.
 *
 * The shared navbar shows, for every module version, whether it is
 * compatible with a given ABI version.
 *
 * Build (from a configured liara-interfaces tree):
 *     cc -std=c11 -I include -o abi-oracle tools/abi-oracle/abi_oracle.c
 *
 * Usage:
 *     abi-oracle < pairs.txt
 *     abi-oracle --self-test
 *
 * Input is one pair per line, `provided required`, each `major.minor.patch`:
 *
 *     1.2.3 1.2.0
 *     0.0.5 0.1.0
 *
 * Output echoes the pair and appends the verdict, so the caller never has
 * to assume the ordering held:
 *
 *     1.2.3 1.2.0 COMPATIBLE
 *     0.0.5 0.1.0 INCOMPATIBLE
 *
 * A malformed line yields `<line> ERROR` on stdout and sets the exit
 * status to 1 once every line has been processed. Failing at the end
 * rather than on the first bad line means a caller batching a hundred
 * pairs learns about all of them at once.
 */

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include <liara/abi_version.h>

enum { LINE_MAX_LENGTH = 256 };

static const char *verdict_name(const liara_version_compat_t compat) {
    switch (compat) {
        case LIARA_VERSION_COMPAT_EXACT:        return "EXACT";
        case LIARA_VERSION_COMPAT_COMPATIBLE:   return "COMPATIBLE";
        case LIARA_VERSION_COMPAT_DEGRADED:     return "DEGRADED";
        case LIARA_VERSION_COMPAT_INCOMPATIBLE: return "INCOMPATIBLE";
        default:                                return "UNKNOWN";
    }
}

/**
 * @brief Parses `major.minor.patch` into a packed version word.
 *
 * Uses the checked constructor rather than the unsafe macro: input reaches
 * this program from a JSON manifest that nothing in the build validates
 * against the field widths, so a component wide enough to overflow its
 * field has to be rejected rather than silently truncated into a
 * neighbouring one.
 *
 * @param[in]  text Null-terminated `major.minor.patch` triple.
 * @param[out] out  Receives the packed version on success.
 * @return Non-zero on success, zero when the text is malformed or a
 *         component does not fit its field.
 */
static int parse_version(const char *const text, uint32_t *const out) {
    unsigned int major = 0U;
    unsigned int minor = 0U;
    unsigned int patch = 0U;
    char trailing = '\0';

    if (sscanf(text, "%u.%u.%u%c", &major, &minor, &patch, &trailing) != 3) { return 0; }

    return liara_try_make_version(major, minor, patch, out) == LIARA_RESULT_SUCCESS;
}

/**
 * @brief Checks the oracle against the pairs ADR 0005 calls out by name.
 *
 * The two disputed pairs are the reason the rule was written down at all:
 * the header and `INTERFACES.md` once disagreed about which side the
 * 0.0.x test applied to. Asserting them here means a future change to the
 * rule cannot pass through this program unnoticed.
 *
 * @return Zero when every case matches, one otherwise.
 */
static int self_test(void) {
    const struct {
        const char *provided;
        const char *required;
        liara_version_compat_t expected;
        const char *why;
    } cases[] = {
        {"1.2.3", "1.2.3", LIARA_VERSION_COMPAT_EXACT,        "identical"},
        {"1.3.0", "1.2.0", LIARA_VERSION_COMPAT_COMPATIBLE,   "newer minor satisfies older"},
        {"1.2.9", "1.2.0", LIARA_VERSION_COMPAT_COMPATIBLE,   "patch is not significant"},
        {"1.1.0", "1.2.0", LIARA_VERSION_COMPAT_DEGRADED,     "older minor is degraded"},
        {"2.0.0", "1.0.0", LIARA_VERSION_COMPAT_INCOMPATIBLE, "major mismatch"},
        {"1.0.0", "2.0.0", LIARA_VERSION_COMPAT_INCOMPATIBLE, "major mismatch, reversed"},
        {"0.0.5", "0.0.5", LIARA_VERSION_COMPAT_EXACT,        "0.0.x equal to itself"},
        {"0.0.5", "0.0.6", LIARA_VERSION_COMPAT_INCOMPATIBLE, "0.0.x demands exact equality"},
        {"0.0.5", "0.1.0", LIARA_VERSION_COMPAT_INCOMPATIBLE, "ADR 0005 disputed pair, provided 0.0.x"},
        {"0.1.0", "0.0.5", LIARA_VERSION_COMPAT_INCOMPATIBLE, "ADR 0005 disputed pair, required 0.0.x"},
        {"0.2.0", "0.1.0", LIARA_VERSION_COMPAT_COMPATIBLE,   "0.x is an ordinary minor comparison"},
    };

    int failures = 0;
    for (size_t i = 0U; i < (sizeof cases / sizeof cases[0]); ++i) {
        uint32_t provided = 0U;
        uint32_t required = 0U;

        if (!parse_version(cases[i].provided, &provided) || !parse_version(cases[i].required, &required)) {
            (void) fprintf(stderr, "FAIL  %s -> %s : unparseable\n", cases[i].provided, cases[i].required);
            ++failures;
            continue;
        }

        const liara_version_compat_t actual = liara_version_provides(provided, required);
        if (actual != cases[i].expected) {
            (void) fprintf(stderr, "FAIL  %s -> %s : expected %s, got %s (%s)\n", cases[i].provided,
                           cases[i].required, verdict_name(cases[i].expected), verdict_name(actual), cases[i].why);
            ++failures;
        }
    }

    (void) fprintf(stderr, "abi-oracle self-test: %zu cases, %d failures\n", sizeof cases / sizeof cases[0], failures);
    return failures == 0 ? 0 : 1;
}

int main(const int argc, char *const argv[]) {
    if (argc > 1 && strcmp(argv[1], "--self-test") == 0) { return self_test(); }
    if (argc > 1) {
        (void) fprintf(stderr, "usage: %s [--self-test] < pairs\n", argv[0]);
        return 2;
    }

    char line[LINE_MAX_LENGTH];
    int malformed = 0;

    while (fgets(line, sizeof line, stdin) != NULL) {
        line[strcspn(line, "\r\n")] = '\0';
        if (line[0] == '\0' || line[0] == '#') { continue; }

        char left[LINE_MAX_LENGTH];
        char right[LINE_MAX_LENGTH];
        uint32_t provided = 0U;
        uint32_t required = 0U;

        if (sscanf(line, "%255s %255s", left, right) != 2 || !parse_version(left, &provided)
            || !parse_version(right, &required)) {
            (void) printf("%s ERROR\n", line);
            malformed = 1;
            continue;
        }

        (void) printf("%s %s %s\n", left, right, verdict_name(liara_version_provides(provided, required)));
    }

    return malformed;
}
