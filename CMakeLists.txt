cmake_minimum_required(VERSION 3.4)

project(tjs 
    VERSION 19.0.0
    LANGUAGES C)

include(CMakePackageConfigHelpers)
include(CMakeDependentOption)
include(GenerateExportHeader)
include(GNUInstallDirs)

cmake_dependent_option(
	TJS_BUILD_PACKAGE
	"Generate package target" ON
	"CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR" OFF
)

if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "No build type selected, default to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()
message(STATUS "Building in ${CMAKE_BUILD_TYPE} mode")
message(STATUS "Building with ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION} on ${CMAKE_SYSTEM}")

set(TJS__VERSION_MAJOR 19)
set(TJS__VERSION_MINOR 0)
set(TJS__VERSION_PATCH 0)
set(TJS__VERSION_SUFFIX "-alpha")
configure_file(
    "${CMAKE_SOURCE_DIR}/src/version.h.in"
    "${CMAKE_SOURCE_DIR}/src/version.h"
)


macro(cpr_option OPTION_NAME OPTION_TEXT OPTION_DEFAULT)
    option(${OPTION_NAME} ${OPTION_TEXT} ${OPTION_DEFAULT})
    if(DEFINED ENV{${OPTION_NAME}})
        # Allow setting the option through an environment variable
        set(${OPTION_NAME} $ENV{${OPTION_NAME}})
    endif()
    if(${OPTION_NAME})
        add_definitions(-D${OPTION_NAME})
    endif()
    message(STATUS "  ${OPTION_NAME}: ${${OPTION_NAME}}")
endmacro()


include(${CMAKE_SOURCE_DIR}/src/js/CMakeLists.txt)
add_subdirectory(deps/quickjs EXCLUDE_FROM_ALL)

option(libuv_buildtests "" OFF)
add_subdirectory(deps/libuv EXCLUDE_FROM_ALL)

option(REPLXX_BUILD_EXAMPLES "" OFF)
option(BUILD_SHARED_LIBS "" OFF)
add_subdirectory(deps/replxx EXCLUDE_FROM_ALL)

cpr_option(DISABLE_WASM "If ON, WASM support will be disabled" OFF)
if (NOT DISABLE_WASM)
    option(BUILD_WASI_SUPPORT "" ON)
    add_subdirectory(deps/wasm3/source EXCLUDE_FROM_ALL)
    target_compile_definitions(m3 PRIVATE
        d_m3LogOutput=0
    )
endif()

cpr_option(DISABLE_CURL "If ON, curl support will be disabled" OFF)
cpr_option(USE_SYSTEM_CURL "If ON, this project will look in the system paths for an installed curl library" ON)

if(NOT DISABLE_CURL)
    if(USE_SYSTEM_CURL)
        find_package(CURL)
    endif()
    if(NOT USE_SYSTEM_CURL OR NOT CURL_FOUND)
        message(STATUS "Not using system Curl, using built-in curl project instead.")
        option(HTTP_ONLY "disables all protocols except HTTP" ON)
        option(BUILD_TESTING "Set to ON to build cURL tests." OFF)
        option(BUILD_CURL_EXE "Set to ON to build cURL executable." OFF)
        add_subdirectory(deps/curl)
        set(CURL_FOUND TRUE)
        set(CURL_LIBRARIES libcurl)
        set(CURL_INCLUDE_DIRS
            ${CURL_SOURCE_DIR}/include
            ${CURL_BINARY_DIR}/include/curl)
    endif()
endif()

# build as a lib and link to lib instead of building executable right away
# add_executable(tjs
#     src/bootstrap.c
#     src/cli.c
#     src/curl-utils.c
#     src/dns.c
#     src/error.c
#     src/fs.c
#     src/misc.c
#     src/modules.c
#     src/process.c
#     src/std.c
#     src/streams.c
#     src/signals.c
#     src/timers.c
#     src/udp.c
#     src/utils.c
#     src/version.c
#     src/vm.c
#     src/wasm.c
#     src/worker.c
#     src/xhr.c
#     ${CMAKE_CURRENT_BINARY_DIR}/js.c
#     deps/quickjs/src/cutils.c
# )

# set_target_properties(tjs PROPERTIES
#     C_STANDARD 11
#     C_STANDARD_REQUIRED ON
# )

# if (CURL_FOUND)
#     if (USE_SYSTEM_CURL)
#         target_compile_definitions(tjs PRIVATE
#             TJS_HAVE_SYSTEM_CURL
#         )
#     endif()
#     target_compile_definitions(tjs PRIVATE
#         TJS_HAVE_CURL
#     )
#     target_include_directories(tjs PRIVATE ${CURL_INCLUDE_DIRS})
#     target_link_libraries(tjs ${CURL_LIBRARIES})
# endif()

# string(TOLOWER ${CMAKE_SYSTEM_NAME} TJS_PLATFORM)
# target_compile_definitions(tjs PRIVATE TJS__PLATFORM="${TJS_PLATFORM}")
# # instruct to link repl statically (different symbol names)
# target_compile_definitions(tjs PRIVATE REPLXX_STATIC)

# if (NOT DISABLE_WASM)
#     target_compile_definitions(tjs PRIVATE
#         TJS_HAVE_WASM
#     )
#     target_link_libraries(tjs m3)
# endif()

# target_link_libraries(tjs replxx qjs uv_a m stdc++)


#####################
# library

add_library(tjs SHARED
    src/bootstrap.c
    #src/cli.c
    src/curl-utils.c
    src/dns.c
    src/error.c
    src/fs.c
    src/misc.c
    src/modules.c
    src/process.c
    src/std.c
    src/streams.c
    src/signals.c
    src/timers.c
    src/udp.c
    src/utils.c
    src/vm.c
    src/wasm.c
    src/worker.c
    src/xhr.c
    # JS files compiled into binary
    ${CMAKE_CURRENT_BINARY_DIR}/js.c
    deps/quickjs/src/cutils.c)

set_target_properties(tjs PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED ON
)

if (CURL_FOUND)
    if (USE_SYSTEM_CURL)
        target_compile_definitions(tjs PRIVATE
            TJS_HAVE_SYSTEM_CURL
        )
    endif()
    target_compile_definitions(tjs PRIVATE
        TJS_HAVE_CURL
    )
    target_include_directories(tjs PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(tjs ${CURL_LIBRARIES})
endif()

string(TOLOWER ${CMAKE_SYSTEM_NAME} TJS_PLATFORM)
target_compile_definitions(tjs PRIVATE TJS__PLATFORM="${TJS_PLATFORM}")
# instruct to link repl statically (different symbol names)
target_compile_definitions(tjs PRIVATE REPLXX_STATIC)


if (NOT DISABLE_WASM)
    target_compile_definitions(tjs PRIVATE
        TJS_HAVE_WASM
    )
    target_link_libraries(tjs m3)
endif()

#set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 -lwinpthread -lcurl ${CMAKE_CXX_STANDARD_LIBRARIES}")
# -lcurl -lssh2

target_link_libraries(tjs replxx qjs uv_a)

#    The "m" library is the C math library on Unix systems, and it resides in
#    a file called "libm.a". The reason this translates to "m.lib" on Windows
#    is that the "lib" prefix is normally added for you on Unix-based
#    toolsets such as GCC.
#
#    With Visual Studio, the math library is part of the standard C runtime
#    library, so there is no additional dependency.


if(NOT WIN32 OR MSYS OR MINGW)
    target_link_libraries(tjs m stdc++)
endif()

# uv needs user32.lib/dll on windows msvc and clang-cl
if(MSVC)
    target_link_libraries(tjs user32)
endif()

# reference: https://gernotklingler.com/blog/creating-using-shared-libraries-different-compilers-different-operating-systems/
include (GenerateExportHeader)
GENERATE_EXPORT_HEADER(tjs           # generates the export header tjs_EXPORTS.h automatically
        BASE_NAME tjs
        EXPORT_MACRO_NAME tjs_EXPORTS
        EXPORT_FILE_NAME tjs_EXPORTS.h
        STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

# cli exe based on dll
add_executable(tjse "src/cli.c")
target_link_libraries(tjse PUBLIC tjs)

set_target_properties(tjs tjse
        PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        )


#### install / package info

# into build dir
#if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set (CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "default install path" FORCE )
#endif()

generate_export_header(tjs)

configure_package_config_file(
	"${PROJECT_SOURCE_DIR}/tjs-config.cmake.in"
	"${PROJECT_BINARY_DIR}/tjs-config.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/tjs
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	NO_SET_AND_CHECK_MACRO
)

write_basic_package_version_file(
	"${PROJECT_BINARY_DIR}/tjs-config-version.cmake"
	COMPATIBILITY AnyNewerVersion
)


target_include_directories(
	qjs
	PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/deps/quickjs/include>
        $<INSTALL_INTERFACE:include/quickjs>
)

# )
# set_property(TARGET qjs APPEND PROPERTY INTERFACE_INCLUDE_DIRECTORIES
#   $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/deps/quickjs/include>
#   $<INSTALL_INTERFACE:include/quickjs>  # <prefix>/include/mylib
# )

install(TARGETS qjs EXPORT tjs-targets)
install(TARGETS uv_a EXPORT tjs-targets)

install(
	TARGETS tjs EXPORT tjs-targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
	EXPORT tjs-targets
	NAMESPACE tjs::
	DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/tjs
)

install(
	FILES
		"${PROJECT_BINARY_DIR}/tjs-config-version.cmake"
		"${PROJECT_BINARY_DIR}/tjs-config.cmake"
	DESTINATION
		${CMAKE_INSTALL_DATADIR}/cmake/tjs
)

# headers
install(
	FILES
        src/tjs.h
        src/tjs_export.h
        src/utils.h
        src/version.h
        include/tjspp.hpp
        deps/quickjs/include/quickjs.h
	DESTINATION
		${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY "deps/libuv/include/" # source directory
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" # target directory
        FILES_MATCHING # install only matched files
        PATTERN "*.h" # select header files
)

if (NOT TJS_BUILD_PACKAGE)
	return()
endif()

include(CPack)

set(CPACK_SET_DESTDIR ON)

set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Txiki.js library")
set(CPACK_PACKAGE_HOMEPAGE_URL "${TJS_URL_INFO_ABOUT}")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")

set(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/LICENSE")

set(CPACK_STRIP_FILES "ON")

set(CPACK_DEBIAN_PACKAGE_SECTION "utilities")