cmake_minimum_required(VERSION 3.8)

# Проверка переменной VCPKG_ROOT
if(DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
else()
    message(FATAL_ERROR "VCPKG_ROOT не задана! Укажите путь к vcpkg.")
endif()

project("archiver")

add_executable(archiver "src/archiver.cpp" "src/main.cpp" "include/archiver.h")

find_package(indicators CONFIG REQUIRED)
target_link_libraries(archiver PRIVATE indicators::indicators)

find_package(ZLIB REQUIRED)
target_link_libraries(archiver PRIVATE ZLIB::ZLIB)