#ifndef API_H
#define API_H

struct api_response {
    int status_code;
    char *body;
};

void make_api_call(const char *url, const char *data, struct api_response *response);

#endif

