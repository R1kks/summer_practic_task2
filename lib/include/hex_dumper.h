/*
hex_dumper.h - файл, в котором определены функции для hex_dumper.c

Дозоров Владимир Евгеньевич
Группа: МК - 101
*/

#pragma once

typedef struct {
    char *file_name;      // Имя файла для чтения (-i)
    long offset;          // Смещение от начала (-o)
    long length;          // Количество байт для вывода (-l, -1 если всё)
    int group_size;       // Размер кусочка (-g)
    int cols_per_line;    // Количество кусочков в строке (-n)
    char *dir_name;       // Имя директории для обработки (-d)
} DumperConfig;
