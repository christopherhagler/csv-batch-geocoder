#include <stdlib.h>
#include <string.h>
#include "csv.h"

void parse_csv(FILE *file, struct row *rows, int *num_rows, int max_rows) {
    char line[1024];
    int i = 0;

    while (fgets(line, sizeof(line), file)) {
        char *place = strtok(line, ",");
        char *street = strtok(NULL, ",");
        char *city = strtok(NULL, ",");
        char *province = strtok(NULL, ",");
        char *postal_code = strtok(NULL, ",");
        char *country_code = strtok(NULL, ",");

	// we require at least a street to perform the search
        if (!street)
            continue;

        size_t place_len = strlen(place);
        size_t street_len = strlen(street);
        size_t city_len = strlen(city);
        size_t province_len = strlen(province);
        size_t postal_code_len = strlen(postal_code);
        size_t country_code_len = strlen(country_code);

        if (place_len >= sizeof(rows[i].place) || street_len >= sizeof(rows[i].street) ||
            city_len >= sizeof(rows[i].city) || province_len >= sizeof(rows[i].province) ||
            postal_code_len >= sizeof(rows[i].postal_code) || country_code_len >= sizeof(rows[i].country_code)) {
            continue;
        }

        memcpy(rows[i].place, place, place_len);
        memcpy(rows[i].street, street, street_len);
        memcpy(rows[i].city, city, city_len);
        memcpy(rows[i].province, province, province_len);
        memcpy(rows[i].postal_code, postal_code, postal_code_len);
        memcpy(rows[i].country_code, country_code, country_code_len);

        rows[i].place[place_len] = '\0';
        rows[i].street[street_len] = '\0';
        rows[i].city[city_len] = '\0';
        rows[i].province[province_len] = '\0';
        rows[i].postal_code[postal_code_len] = '\0';
        rows[i].country_code[country_code_len] = '\0';

        (*num_rows)++;
        i++;

        if (*num_rows >= max_rows) {
            break;
        }
    }
}
