# Compiler settings shared by every Liara module.
#
# This file lives in liara-interfaces because every module already depends on it, by add_subdirectory
# in the workspace and by find_package standalone.
#
# Everything here is PRIVATE. Warning levels, optimisation and debug definitions are a module's own
# business; the only thing that legitimately crosses to a consumer is the C ABI, and it carries no
# flags.

include_guard(GLOBAL)

if(DEFINED ENV{CI} AND NOT DEFINED LIARA_WERROR)
    set(LIARA_WERROR_DEFAULT ON)
else()
    set(LIARA_WERROR_DEFAULT OFF)
endif()

option(LIARA_WERROR "Treat compiler warnings as errors." ${LIARA_WERROR_DEFAULT})

option(LIARA_IPO
        "Enable interprocedural optimisation (LTO) in optimised configurations. Off by default: it \
makes static libraries readable only by the compiler that produced them, which is a property the \
published release artifacts must choose deliberately rather than inherit."
        OFF)

function(liara_configure_target target)
    target_compile_features(${target} PRIVATE cxx_std_20)

    set(warnings_c_cxx
            -Wall -Wextra -Wpedantic
            -Wshadow                 # a local hiding a member is almost never intended
            -Wcast-align             # alignment-increasing casts are UB waiting for an architecture
            -Wformat=2
            -Wimplicit-fallthrough
            -Wdouble-promotion       # a silent float->double in a hot loop is a real cost
            -Wundef                  # #if on an undefined macro silently means zero
    )

    set(warnings_cxx_only
            -Wnon-virtual-dtor
            -Woverloaded-virtual
            -Wold-style-cast         # the ABI shim is the one place casts are unavoidable; make them loud
    )

    set(warnings_gnu_only
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wmisleading-indentation
    )

    set(warnings_msvc
            /W4
            /permissive-             # conformance mode; without it MSVC accepts code nothing else does
            /Zc:__cplusplus          # otherwise __cplusplus reports C++98 and every feature test lies
            /Zc:preprocessor         # conformant preprocessor: __VA_OPT__ and the ABI macros need it
            /utf-8
    )

    if(MSVC)
        if(MSVC_VERSION LESS 1936)   # Visual Studio 2022 17.6
            message(FATAL_ERROR
                    "Liara requires MSVC 19.36 (Visual Studio 2022 17.6) or newer. "
                    "Detected: ${MSVC_VERSION}. See TOOLING.md section 1.")
        endif()

        target_compile_options(${target} PRIVATE ${warnings_msvc})

        # /MP parallelises within a translation-unit batch and is a Visual Studio generator concern;
        # Ninja already parallelises at the file level and would fight it.
        if(CMAKE_GENERATOR MATCHES "Visual Studio")
            target_compile_options(${target} PRIVATE /MP)
        endif()

        if(LIARA_WERROR)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    else()
        target_compile_options(${target} PRIVATE
                ${warnings_c_cxx}
                $<$<COMPILE_LANGUAGE:CXX>:${warnings_cxx_only}>
                $<$<CXX_COMPILER_ID:GNU>:$<$<COMPILE_LANGUAGE:CXX>:${warnings_gnu_only}>>
        )

        if(LIARA_WERROR)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    endif()

    target_compile_definitions(${target} PRIVATE
            $<$<CONFIG:Debug>:LIARA_DEBUG=1>
            $<$<CONFIG:Debug>:LIARA_ENABLE_ASSERTS=1>
    )

    if(LIARA_IPO)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
        set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO ON)
    endif()

    set_target_properties(${target} PROPERTIES
            C_VISIBILITY_PRESET hidden
            CXX_VISIBILITY_PRESET hidden
            VISIBILITY_INLINES_HIDDEN ON
            POSITION_INDEPENDENT_CODE ON
    )
endfunction()
