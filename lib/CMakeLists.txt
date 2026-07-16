#CMakeLists.txt - настройка сборки библиотеки
#
#Дозоров Владимир Евгеньевич
#Группа: МК - 101

add_library(hex_dumper_lib
    src/hex_dumper.c
    include/hex_dumper.h
)

target_include_directories(hex_dumper_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)