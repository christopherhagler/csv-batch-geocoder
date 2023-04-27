#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csv.h"
#include "api.h"

#define MAX_ROWS_PER_BATCH 1000
#define BATCH_DELAY_SECONDS 1

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_csv> <api_url>\n", argv[0]);
        exit(1);
    }

    const char *input_csv_path = argv[1];
    const char *api_url = argv[2];

    FILE *input_csv = fopen(input_csv_path, "r");
    if (!input_csv) {
        perror("Failed to open input CSV file");
        exit(1);
    }

    struct row rows[MAX_ROWS_PER_BATCH];
    int num_rows = 0;

    while (!feof(input_csv)) {
        parse_csv(input_csv, rows, &num_rows, MAX_ROWS_PER_BATCH);

        if (num_rows == 0)
            continue;

        for (int i = 0; i < num_rows; i++) {
            char args[2048];
            snprintf(
            	args, 
            	sizeof(args), 
		"%s %s %s %s %s %s",
            	rows[i].place, 
            	rows[i].street, 
            	rows[i].city, 
            	rows[i].province, 
            	rows[i].postal_code, 
            	rows[i].country_code
            );
            struct api_response response;
            make_api_call_get(api_url, args, &response);

            printf("API response status code: %d\n", response.status_code);
            printf("API response body:\n%s\n", response.body);

            free(response.body);
            sleep(BATCH_DELAY_SECONDS);
        }

        num_rows = 0;
    }

    fclose(input_csv);

    return 0;
}

