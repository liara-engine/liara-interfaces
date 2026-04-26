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
