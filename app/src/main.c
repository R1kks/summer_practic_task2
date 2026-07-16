/*
main.c - файл, который содержит главную функцию main

Дозоров Владимир Евгеньевич
Группа: МК - 101
*/

#include "hex_dumper.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    DumperConfig cfg;

    if (!parse_arguments(argc, argv, &cfg)) {
        return 1;
    }
    return 0;
}