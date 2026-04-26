#pragma once

#define LIARA_MAKE_VERSION(major, minor, patch) \
(((uint32_t)(major) << 22) | ((uint32_t)(minor) << 12) | (uint32_t)(patch))

#define LIARA_VERSION_MAJOR(version)  (((uint32_t)(version) >> 22) & 0x3FFu)
#define LIARA_VERSION_MINOR(version)  (((uint32_t)(version) >> 12) & 0x3FFu)
#define LIARA_VERSION_PATCH(version)  ((uint32_t)(version) & 0xFFFu)
