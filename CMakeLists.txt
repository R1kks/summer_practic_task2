#CMakeLists.txt - корневой файл сборки проекта
#
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

cmake_minimum_required(VERSION 3.10)
project(HexDumperProject C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Подключаем макрос сборки исполняемых файлов
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/MyFunction.cmake)

add_subdirectory(lib)
add_subdirectory(app)