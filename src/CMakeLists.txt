cmake_minimum_required(VERSION 3.12...3.20)


set(PROJECT_COMPILE_FLAG )
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_definitions(-DNOMINMAX)
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} /W4 /WX /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
else()
    set(PROJECT_COMPILE_FLAG "${CMAKE_CXX_FLAGS} -Wall -Werror")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU" AND UNIX)
        set(PROJECT_COMPILE_FLAG "${PROJECT_COMPILE_FLAG} -lpthread")
    endif()
endif()

add_compile_definitions(
    SPDLOG_COMPILED_LIB
    SPDLOG_NO_EXCEPTIONS
)

add_subdirectory(lib)
add_subdirectory(prj_exec1)