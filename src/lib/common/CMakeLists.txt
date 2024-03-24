cmake_minimum_required(VERSION 3.12...3.20)
project(common)
# source code dir
aux_source_directory(${PROJECT_SOURCE_DIR}/src src)
file(GLOB include_file "${CMAKE_SOURCE_DIR}/include/lib/${PROJECT_NAME}/*.h")
add_library(${PROJECT_NAME} SHARED
    ${src} 
    ${include_file}
 )
# setting up include dir
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${INCLUDE_BASE}/lib/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    spdlog::spdlog
)

add_dependencies(${PROJECT_NAME} spdlog::spdlog)