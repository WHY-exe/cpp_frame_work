cmake_minimum_required(VERSION 3.12...3.20)
#  unit-test
project(unit_test)

file(GLOB include_file "${INCLUDE_BASE}/${PROJECT_NAME}/*.h")
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src main)
add_executable(${PROJECT_NAME} ${main} ${include_file})
# setting up include dir
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${INCLUDE_BASE}/${PROJECT_NAME}
    ${THIRD_PARTY_INCLUDE}/Catch2/src
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    common
    spdlog::spdlog
    Catch2
    Catch2::Catch2WithMain
)

add_dependencies(${PROJECT_NAME} 
    common 
    spdlog::spdlog 
    Catch2 
    Catch2::Catch2WithMain)