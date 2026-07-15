#include <stdint.h>
#include <liara/abi_version.h>
#include <liara/version.h>

int main(void) {
    uint32_t version;
    if (!liara_try_make_version(1, 2, 3, &version)) {
        return 1;
    }
    if (LIARA_VERSION_MAJOR(version) != 1 ||
        LIARA_VERSION_MINOR(version) != 2 ||
        LIARA_VERSION_PATCH(version) != 3) {
        return 1;
    }

    uint32_t interface_version = LIARA_ABI_VERSION;
    if (LIARA_VERSION_MAJOR(interface_version) != 0 ||
        LIARA_VERSION_MINOR(interface_version) != 0 ||
        LIARA_VERSION_PATCH(interface_version) != 1) {
        return 1;
    }

    return 0;
}
