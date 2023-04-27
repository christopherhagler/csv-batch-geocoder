#ifndef API_H
#define API_H

struct api_response {
    int status_code;
    char *body;
};

void make_api_call_get(const char *url, char *parameters, struct api_response *response);

#endif

