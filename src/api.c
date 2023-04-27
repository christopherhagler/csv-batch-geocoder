#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include "api.h"

size_t curl_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t realsize = size * nmemb;
    struct api_response *response = (struct api_response *)userdata;

    response->body = realloc(response->body, response->status_code + realsize + 1);
    if (response->body) {
        memcpy(&(response->body[response->status_code]), ptr, realsize);
        response->status_code += realsize;
        response->body[response->status_code] = 0;
    }

    return realsize;
}

void make_api_call(const char *url, const char *data, struct api_response *response) {
    CURL *curl = curl_easy_init();
    if (curl) {
        response->status_code = 0;
        response->body = malloc(1);

        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)response);

        curl_easy_perform(curl);

        curl_easy_cleanup(curl);
    }
}

