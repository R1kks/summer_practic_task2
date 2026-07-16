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

static int dump_single_file(const char *filepath, const DumperConfig *cfg) {
    FILE *file = fopen(filepath, "rb");
    if (!file) {
        fprintf(stderr, "Ошибка: Не удалось открыть файл %s\n", filepath);
        return 0;
    }

    if (cfg->offset > 0 && fseek(file, cfg->offset, SEEK_SET) != 0) {
        fprintf(stderr, "Ошибка: Не удалось перейти к смещению %ld в файле %s\n", cfg->offset, filepath);
        fclose(file);
        return 0;
    }

    int line_sz = cfg->cols_per_line * cfg->group_size;
    unsigned char *buf = (unsigned char *)malloc(line_sz);
    long remaining = cfg->length;
    long curr_offset = cfg->offset;

    while (1) {
        int to_read = (cfg->length != -1 && remaining < line_sz) ? (int)remaining : line_sz;
        if (to_read <= 0) break;

        int read = (int)fread(buf, 1, to_read, file);
        if (read <= 0) break;

        printf("%08lx", curr_offset);
        print_spaces(2);

        for (int col = 0; col < cfg->cols_per_line; col++) {
            int idx = col * cfg->group_size;

            if (idx < read) {
                int actual = (idx + cfg->group_size <= read) ? cfg->group_size : (read - idx);

                for (int i = 0; i < cfg->group_size - actual; i++) {
                    printf("%c%c", '0', '0');
                }

                for (int i = actual - 1; i >= 0; i--) {
                    char hex[2];
                    byte_to_hex_chars(buf[idx + i], hex);
                    printf("%c%c", hex[0], hex[1]);
                }
                print_spaces(1);
            } else {
                print_spaces(cfg->group_size * 2 + 1);
            }
        }

        if (cfg->group_size == 1) {
            printf("%c%c", '|', ' ');
            for (int i = 0; i < read; i++) {
                char c = (buf[i] >= 32 && buf[i] <= 126) ? buf[i] : '.';
                printf("%c", c);
            }
        }

        printf("%c", '\n');

        curr_offset += read;
        if (cfg->length != -1) {
            remaining -= read;
        }
    }

    free(buf);
    fclose(file);
    return 1;
}

static void process_dir_entry(const char *dir, const char *name, const DumperConfig *cfg) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, name);
    
    struct stat st;
    if (stat(path, &st) == 0 && S_ISREG(st.st_mode)) {
        printf("--- Файл: %s ---\n", name);
        dump_single_file(path, cfg);
        printf("%c", '\n');
    }
}