#ifndef CSV_H
#define CSV_H

#include <stdio.h>

struct row {
    char place[128];
    char street[128];
    char city[64];
    char province[32];
    char postal_code[16];
    char country_code[16];
};

void parse_csv(FILE *file, struct row *rows, int *num_rows, int max_rows);

#endif

