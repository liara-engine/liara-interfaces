cmake_minimum_required(VERSION 3.29)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_BINARY_DIR)
    message(FATAL_ERROR
            "In-source builds are not supported. Build into build/<preset-name>/ instead, which is "
            "what the presets already do:\n"
            "    cmake --preset linux-debug-clang\n"
            "Remove the CMakeCache.txt and CMakeFiles/ this attempt just created before retrying.")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(LiaraCompilerSettings)
include(GNUInstallDirs)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.release-please-manifest.json" RELEASE_PLEASE_MANIFEST)
string(JSON PROJECT_VERSION GET ${RELEASE_PLEASE_MANIFEST} ".")
message(STATUS "LiaraInterfaces version: ${PROJECT_VERSION}")

project(LiaraInterfaces
        VERSION ${PROJECT_VERSION}
        DESCRIPTION "C ABI headers shared by all Liara modules"
        HOMEPAGE_URL "https://github.com/liara-engine/liara-interfaces"
        LANGUAGES C CXX
)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/include/liara/config.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/generated/liara/config.h"
        @ONLY
)

if(PROJECT_IS_TOP_LEVEL)
    set(LIARA_INTERFACES_IS_TOP_LEVEL ON)
else()
    set(LIARA_INTERFACES_IS_TOP_LEVEL OFF)
endif()

option(LIARA_INTERFACES_BUILD_TESTS
        "Build the liara-interfaces test suite."
        ${LIARA_INTERFACES_IS_TOP_LEVEL})
option(LIARA_INTERFACES_INSTALL
        "Generate install targets for liara-interfaces."
        ${LIARA_INTERFACES_IS_TOP_LEVEL})

if(LIARA_INTERFACES_IS_TOP_LEVEL)
    set(CMAKE_C_STANDARD 11)
    set(CMAKE_C_STANDARD_REQUIRED ON)
    set(CMAKE_C_EXTENSIONS OFF)

    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()

add_library(liara_interfaces INTERFACE)
set_target_properties(liara_interfaces PROPERTIES EXPORT_NAME Interfaces)
add_library(Liara::Interfaces ALIAS liara_interfaces)

target_include_directories(liara_interfaces INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(LIARA_INTERFACES_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(LIARA_INTERFACES_INSTALL)
    include(CMakePackageConfigHelpers)

    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            COMPONENT liara-interfaces-dev
            FILES_MATCHING
            PATTERN "*.h"
            PATTERN "config.h.in" EXCLUDE
    )

    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/liara/config.h"
            DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/liara"
            COMPONENT liara-interfaces-dev
    )

    install(TARGETS liara_interfaces EXPORT LiaraInterfacesTargets)

    install(EXPORT LiaraInterfacesTargets
            FILE LiaraInterfacesTargets.cmake
            NAMESPACE Liara::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LiaraInterfaces
            COMPONENT liara-interfaces-dev
    )

    write_basic_package_version_file(
            "${CMAKE_CURRENT_BINARY_DIR}/LiaraInterfacesConfigVersion.cmake"
            VERSION ${PROJECT_VERSION}
            COMPATIBILITY SameMajorVersion
            ARCH_INDEPENDENT
    )

    configure_package_config_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LiaraInterfacesConfig.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/LiaraInterfacesConfig.cmake"
            INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LiaraInterfaces
    )

    install(FILES
            "${CMAKE_CURRENT_BINARY_DIR}/LiaraInterfacesConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/LiaraInterfacesConfigVersion.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LiaraInterfaces
            COMPONENT liara-interfaces-dev
    )

    install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/cmake/LiaraCompilerSettings.cmake"
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LiaraInterfaces
            COMPONENT liara-interfaces-dev
    )
endif()
