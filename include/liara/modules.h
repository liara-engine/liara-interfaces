/**
 * @file modules.h
 * @brief Liara modules interface header file.
 */

#pragma once

#include <liara/private_utils.h>

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Structure representing information about a Liara module.
 *
 * This structure contains information about a Liara module, including its version, ABI version, and name.
 * It is used to provide metadata about the module to the Liara framework.
 */
LIARA_TYPEDEF_STRUCT(liara_module_info_t,
    uint32_t struct_version;        /**< Version of the liara_module_info_t structure. */
    uint32_t abi_version;           /**< ABI version of the module. */
    uint32_t module_version;        /**< Version of the module. */
    uint32_t reserved;              /**< Reserved for future use. */
    const char* module_name;        /**< Name of the module. */
    const char* abi_version_str;    /**< String representation of the ABI version. */
    const char* module_version_str; /**< String representation of the module version. */
);

#define LIARA_MODULE_INFO_VERSION 1U /**< Current version of the liara_module_info_t structure. */

#ifdef __cplusplus
}
#endif
