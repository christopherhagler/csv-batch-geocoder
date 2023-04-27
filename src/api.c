#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "api.h"

static size_t write_response(void *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    char *data = (char *)userdata;
    data = realloc(data, strlen(data) + realsize + 1);
    if (data) {
        memcpy(data + strlen(data), ptr, realsize);
        data[strlen(data) + realsize] = '\0';
    }
    return realsize;
}

void make_api_call_get(const char *url, char *parameters, struct api_response *response) {
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl) {
        char *clean_parameters = curl_easy_escape(curl, parameters, strlen(parameters));
        char full_url[2048];
        snprintf(full_url, sizeof(full_url), "%s?text=%s&size=1", url, clean_parameters);

        curl_easy_setopt(curl, CURLOPT_URL, full_url);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_response);

        // Allocate a separate buffer for the response data
        char *response_data = malloc(4096);
        *response_data = '\0';

        // Set the response buffer as userdata
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, response_data);

        curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5L);

        printf("url: %s \n", full_url);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            response->status_code = -1;
        } else {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response->status_code);
        }
        curl_free(clean_parameters);
        curl_easy_cleanup(curl);

        // Assign the response buffer to the response struct
        response->body = response_data;
    }
}


