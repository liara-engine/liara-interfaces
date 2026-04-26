cmake_minimum_required(VERSION 3.29)

project(LiaraInterfaces
        VERSION 0.0.0
        DESCRIPTION "C ABI headers shared by all Liara modules"
        HOMEPAGE_URL "https://github.com/liara-engine/liara-interfaces"
        LANGUAGES C CXX
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
add_library(Liara::Interfaces ALIAS liara_interfaces)

target_include_directories(liara_interfaces INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

if(LIARA_INTERFACES_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(LIARA_INTERFACES_INSTALL)
    include(GNUInstallDirs)
    include(CMakePackageConfigHelpers)

    install(DIRECTORY include/
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            FILES_MATCHING PATTERN "*.h"
    )

    install(TARGETS liara_interfaces EXPORT LiaraInterfacesTargets)

    install(EXPORT LiaraInterfacesTargets
            FILE LiaraInterfacesTargets.cmake
            NAMESPACE Liara::
            DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LiaraInterfaces
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
    )
endif()
