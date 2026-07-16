#include "hex_dumper.h"
#include "getopt.h" 
#include <stdio.h>
#include <stdlib.h>


static void byte_to_hex_chars(unsigned char byte, char out[2]) {
    const char hex_digits[] = "0123456789ABCDEF";
    out[0] = hex_digits[(byte >> 4) & 0x0F];
    out[1] = hex_digits[byte & 0x0F];
}

static void print_spaces(int count) {
    for (int i = 0; i < count; i++) {
        printf("%c", ' ');
    }
}

int parse_arguments(int argc, char *argv[], DumperConfig *cfg) {
    cfg->file_name = NULL;
    cfg->dir_name = NULL;
    cfg->offset = 0;
    cfg->length = -1;
    cfg->group_size = 1;
    cfg->cols_per_line = 16;

    int opt;
    while ((opt = getopt(argc, argv, "i:o:l:g:n:d:")) != -1) {
        switch (opt) {
            case 'i':
                cfg->file_name = optarg;
                break;
            case 'o':
                cfg->offset = atol(optarg);
                break;
            case 'l':
                cfg->length = atol(optarg);
                break;
            case 'g':
                cfg->group_size = atoi(optarg);
                break;
            case 'n':
                cfg->cols_per_line = atoi(optarg);
                break;
            case 'd':
                cfg->dir_name = optarg;
                break;
            default:
                fprintf(stderr, "Использование: %s [-i файл] [-o смещение] [-l длина] [-g размер_группы] [-n колонок] [-d папка]\n", argv[0]);
                return 0;
        }
    }

    if (cfg->group_size <= 0 || cfg->cols_per_line <= 0) {
        fprintf(stderr, "Ошибка: Размеры группы (-g) и колонок (-n) должны быть больше 0.\n");
        return 0;
    }
    return 1;
}
