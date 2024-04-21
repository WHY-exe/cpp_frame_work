# cmake root file
cmake_minimum_required(VERSION 3.12...3.20)
# enter your project name here
project(cpp_frame)
# specify c++ standar
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# msvc compile option
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # set debug compile option
    if (POLICY CMP0141)
        cmake_policy(SET CMP0141 NEW)
        set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF: $< AND: $<C_COMPILER_ID:MSVC>, $<CXX_COMPILER_ID:MSVC> >, $< $<CONFIG:Debug,RelWithDebInfo>:EditAndContinue >, $< $<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()
    # active multi-thread compliation flag on msvc
    cmake_host_system_information(RESULT CPU_NUMBER_OF_LOGICAL_CORES QUERY NUMBER_OF_LOGICAL_CORES)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${CPU_NUMBER_OF_LOGICAL_CORES}")
    add_compile_definitions(
        _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
    )
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
endif()
# public include dir
set(INCLUDE_BASE "${PROJECT_SOURCE_DIR}/include")
set(THIRD_PARTY_INCLUDE "${PROJECT_SOURCE_DIR}/third-party")
include_directories(
    ${INCLUDE_BASE}
    ${INCLUDE_BASE}/lib
    ${THIRD_PARTY_INCLUDE}/spdlog/include
)
# binary output dir
set(BIN_DIR "${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
# static lib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${BIN_DIR}/static_lib")
# shared lib
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${BIN_DIR}")
# executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${BIN_DIR}")
# add library linking dir
link_directories(
    ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
    ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
)

add_subdirectory(src)
add_subdirectory(third-party)

option(DO_TEST "Whether compile unit test" on)
if(DO_TEST)
    enable_testing()
    add_subdirectory(test)
    add_test(NAME unit_test COMMAND unit_test)
endif()
