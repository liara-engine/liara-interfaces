#include <stdint.h>
#include <liara/version.h>

int main(void) {
    uint32_t version = LIARA_MAKE_VERSION(1, 2, 3);
    if (LIARA_VERSION_MAJOR(version) == 1 &&
        LIARA_VERSION_MINOR(version) == 2 &&
        LIARA_VERSION_PATCH(version) == 3) {
        return 0;
    }

    return 1;
}
