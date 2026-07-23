find_package(doctest CONFIG REQUIRED)

add_executable(test_interfaces_cxx test_version_cxx.cpp)
target_link_libraries(test_interfaces_cxx PRIVATE
        Liara::Interfaces
        doctest::doctest)
add_test(NAME test_interfaces_cxx COMMAND test_interfaces_cxx)

add_executable(test_interfaces_c test_version_c.c)
target_link_libraries(test_interfaces_c PRIVATE Liara::Interfaces)
set_target_properties(test_interfaces_c PROPERTIES
        C_STANDARD          11
        C_STANDARD_REQUIRED ON
        C_EXTENSIONS        OFF)
add_test(NAME test_interfaces_c COMMAND test_interfaces_c)

find_program(ZIG_EXECUTABLE NAMES zig)
if(ZIG_EXECUTABLE)
    if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
        add_test(NAME test_zig_linux
                COMMAND
                ${ZIG_EXECUTABLE} run test.zig
                -I${PROJECT_SOURCE_DIR}/include
                -I${CMAKE_CURRENT_BINARY_DIR}/../generated
                -lc
                -target x86_64-linux-gnu
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/zig
        )
    elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        add_test(NAME test_zig_windows
                COMMAND
                ${ZIG_EXECUTABLE} run test.zig
                -I${PROJECT_SOURCE_DIR}/include
                -I${CMAKE_CURRENT_BINARY_DIR}/../generated
                -lc
                -target x86_64-windows-gnu
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/zig
        )
    endif()
endif()

find_program(RUST_EXECUTABLE NAMES cargo)
if(RUST_EXECUTABLE)
    add_test(NAME test_rust
            COMMAND
            ${CMAKE_COMMAND} -E env
            LIARA_INCLUDE_DIR=${PROJECT_SOURCE_DIR}/include
            LIARA_GENERATED_INCLUDE_DIR=${CMAKE_CURRENT_BINARY_DIR}/../generated
            ${RUST_EXECUTABLE} run
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/rust
    )
endif()
