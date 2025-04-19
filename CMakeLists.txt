cmake_minimum_required(VERSION 3.8)

# Указываем путь к vcpkg
if(NOT CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "E:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
endif()

project("archiver")

# Добавьте источник в исполняемый файл этого проекта.
add_executable(archiver "src/archiver.cpp" "include/archiver.h" "src/main.cpp")

if(CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET archiver PROPERTY CXX_STANDARD 20)
endif()

# Поиск zlib с помощью find_package
find_package(ZLIB REQUIRED)

# Проверка, был ли найден zlib
if(ZLIB_FOUND)
    message(STATUS "zlib найден: ${ZLIB_INCLUDE_DIRS}")
else()
    message(FATAL_ERROR "zlib не найден!")
endif()

# Использование zlib
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(archiver ${ZLIB_LIBRARIES})
