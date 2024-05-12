cmake_minimum_required(VERSION 3.12...3.20)
set(PROJECT_COMPILE_FLAG )
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DNOMINMAX)
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} /W4 /WX /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} /Ox")
    endif()
else()
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} -Wall -Werror")
    if ("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} -O0 -g -ggdb")
    else()
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} -O3")
    endif()
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND UNIX)
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} -lpthread")
    endif()
endif()

add_compile_definitions(
    SPDLOG_COMPILED_LIB
    SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE
    # SPDLOG_NO_EXCEPTIONS
)

include_directories(
    ${INCLUDE_BASE}
    ${INCLUDE_BASE}/lib
)

add_subdirectory(lib)
add_subdirectory(prj_exec1)