cmake_minimum_required(VERSION 3.4)

project(quickjs LANGUAGES C)

include(CheckCCompilerFlag)

# Compiler check
string(CONCAT is-msvc $<OR:
$<C_COMPILER_ID:MSVC>,
$<STREQUAL:${CMAKE_C_COMPILER_FRONTEND_VARIANT},MSVC>
>)

set(qjs_cflags -Wall)
if(CMAKE_C_COMPILER_ID MATCHES "AppleClang|Clang")
    list(APPEND qjs_cflags
        -Wextra
        -Wno-sign-compare
        -Wno-missing-field-initializers
        -Wno-unused-parameter
        -Wno-unused-variable
        -funsigned-char)
else()
    list(APPEND qjs_cflags
#        -Wno-array-bounds
#        -Wno-unused-variable
#        -Wno-unused-but-set-variable
            )
endif()

#check_c_compiler_flag(/W4 QJS_LINT_W4)
#check_c_compiler_flag(-Wall QJS_LINT_WALL) # DO NOT use this under MSVC
#
## TODO: Place these into its own function
#check_c_compiler_flag(-Wno-unused-parameter QJS_LINT_NO_UNUSED_PARAMETER)
#check_c_compiler_flag(-Wstrict-prototypes QJS_LINT_STRICT_PROTOTYPES)
#check_c_compiler_flag(-Wextra QJS_LINT_EXTRA)
#
#set(lint-no-unused-parameter $<$<BOOL:${QJS_LINT_NO_UNUSED_PARAMETER}>:-Wno-unused-parameter>)
#set(lint-no-unused-variable $<$<BOOL:${QJS_LINT_NO_UNUSED_VARIABLE}>:-Wno-unused-variable>)
#set(lint-strict-prototypes $<$<BOOL:${QJS_LINT_STRICT_PROTOTYPES}>:-Wstrict-prototypes>)
#set(lint-extra $<$<BOOL:${QJS_LINT_EXTRA}>:-Wextra>)
#set(lint-w4 $<$<BOOL:${QJS_LINT_W4}>:/W4>)
## Unfortunately, this one is complicated because MSVC and clang-cl support -Wall
## but using it is like calling -Weverything
#string(CONCAT lint-default $<
#$<AND:$<BOOL:${QJS_LINT_WALL}>,$<NOT:${is-msvc}>>:-Wall
#>)
#
#list(APPEND qjs_cflags ${lint-strict-prototypes} ${lint-extra} ${lint-default} ${lint-w4})
#list(APPEND qjs_cflags ${lint-no-unused-parameter} ${lint-no-unused-variable})

file(STRINGS "VERSION" QJS_VERSION_STR)

add_library(qjs STATIC
    src/cutils.c
    src/libbf.c
    src/libregexp.c
    src/libunicode.c
    src/quickjs.c
)
set_target_properties(qjs PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
    CMAKE_C_EXTENSIONS ON
)
target_compile_options(qjs PRIVATE ${qjs_cflags})
target_compile_definitions(qjs PUBLIC
    QJS_VERSION_STR="${QJS_VERSION_STR}"
)
target_compile_definitions(qjs PRIVATE
    CONFIG_BIGNUM
    CONFIG_VERSION="${QJS_VERSION_STR}"
)
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(qjs PRIVATE
        DUMP_LEAKS
    )
endif()

#target_include_directories(qjs PUBLIC include)
target_include_directories(qjs PRIVATE src)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    target_link_libraries(qjs atomic)
endif()
