#include "wrapper.h"

uint32_t liara_rs_make_version(uint32_t major, uint32_t minor, uint32_t patch) {
    return liara_make_version(major, minor, patch);
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

uint32_t liara_rs_interface_version(void) {
    return liara_interface_version();
}
