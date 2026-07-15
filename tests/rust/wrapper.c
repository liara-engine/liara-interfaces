#include "wrapper.h"

bool liara_rs_try_make_version(uint32_t major, uint32_t minor, uint32_t patch, uint32_t *version) {
    return liara_try_make_version(major, minor, patch, version);
}

uint32_t liara_rs_version_major(uint32_t version) {
    return liara_version_major(version);
}

uint32_t liara_rs_version_minor(uint32_t version) {
    return liara_version_minor(version);
}

uint32_t liara_rs_version_patch(uint32_t version) {
    return liara_version_patch(version);
}

uint32_t liara_rs_abi_version(void) {
    return liara_abi_version();
}
