cmake_minimum_required(VERSION 3.11...3.20)
if(WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()
add_subdirectory(common)

# add subdirectory for other third party lib