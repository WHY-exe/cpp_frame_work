cmake_minimum_required(VERSION 3.12...3.20)
# 这个项目输出可执行文件
project(prj_exec1)
file(GLOB include_file "${INCLUDE_BASE}/${PROJECT_NAME}/*.h")
aux_source_directory(${PROJECT_SOURCE_DIR}/src main)

add_executable(${PROJECT_NAME} 
    ${main} 
    ${include_file}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${INCLUDE_BASE}/${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    common
    spdlog::spdlog
)

add_dependencies(${PROJECT_NAME} common spdlog::spdlog)