find_package(doctest CONFIG REQUIRED)

option(LIARA_INTERFACES_REQUIRE_CROSS_LANGUAGE_TESTS
        "Fail configuration if the Zig or Rust toolchain is missing, instead of skipping the \
cross-language tests. Off for developers, on for the one CI leg responsible for them." OFF)

find_program(LIARA_ZIG_EXECUTABLE zig)
find_program(LIARA_CARGO_EXECUTABLE cargo)

foreach(tool IN ITEMS LIARA_ZIG_EXECUTABLE LIARA_CARGO_EXECUTABLE)
    if(NOT ${tool})
        if(LIARA_INTERFACES_REQUIRE_CROSS_LANGUAGE_TESTS)
            message(FATAL_ERROR "${tool} not found, but cross-language tests are required here.")
        endif()
        message(STATUS "${tool} not found: the matching cross-language tests are skipped.")
    endif()
    message(STATUS "${tool} found: ${${tool}}")
endforeach()

add_executable(test_interfaces_cxx test_version_cxx.cpp)
target_link_libraries(test_interfaces_cxx PRIVATE
        Liara::Interfaces
        doctest::doctest)
add_test(NAME test_interfaces_cxx COMMAND test_interfaces_cxx)
set_tests_properties(test_interfaces_cxx PROPERTIES LABELS "unit")

add_executable(test_interfaces_c test_version_c.c)
target_link_libraries(test_interfaces_c PRIVATE Liara::Interfaces)
set_target_properties(test_interfaces_c PROPERTIES
        C_STANDARD          11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS        OFF)
add_test(NAME test_interfaces_c COMMAND test_interfaces_c)
set_tests_properties(test_interfaces_c PROPERTIES LABELS "unit")

if(LIARA_ZIG_EXECUTABLE)
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        add_test(NAME test_zig_linux
                COMMAND
                ${LIARA_ZIG_EXECUTABLE} run test.zig
                -I${PROJECT_SOURCE_DIR}/include
                -I${CMAKE_CURRENT_BINARY_DIR}/../generated
                -lc
                -target x86_64-linux-gnu
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/zig
        )
        set_tests_properties(test_zig_linux PROPERTIES LABELS "unit;cross-lang")
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        add_test(NAME test_zig_windows
                COMMAND
                ${LIARA_ZIG_EXECUTABLE} run test.zig
                -I${PROJECT_SOURCE_DIR}/include
                -I${CMAKE_CURRENT_BINARY_DIR}/../generated
                -lc
                -target x86_64-windows-gnu
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/zig
        )
        set_tests_properties(test_zig_windows PROPERTIES LABELS "unit;cross-lang")
    endif()
endif()

find_program(LIARA_CARGO_EXECUTABLE NAMES cargo)
if(LIARA_CARGO_EXECUTABLE)
    add_test(NAME test_rust
            COMMAND
            ${CMAKE_COMMAND} -E env
            LIARA_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
            LIARA_GENERATED_INCLUDE_DIR=${CMAKE_CURRENT_BINARY_DIR}/../generated
            ${LIARA_CARGO_EXECUTABLE} run
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rust
    )
    set_tests_properties(test_rust PROPERTIES LABELS "unit;cross-lang")
endif()
