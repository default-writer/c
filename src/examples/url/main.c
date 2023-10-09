#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct memory {
    char* response;
    size_t size;
};

size_t callback(void* data, size_t size, size_t nmemb, void* clientp) {
    size_t realsize = size * nmemb;
    struct memory* mem = (struct memory*)clientp;

    char* ptr = realloc(mem->response, mem->size + realsize + 1);
    if (ptr == NULL) return 0;

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

int fetch(const char* url, struct memory* chunk) {
    CURLcode res;
    CURL* curl_handle = curl_easy_init();
    curl_easy_setopt(curl_handle, CURLOPT_SSLVERSION, CURL_SSLVERSION_TLSv1_3);
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, callback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, chunk);
    res = curl_easy_perform(curl_handle);
    if (res != CURLE_OK) {
        fprintf(stderr, "fetch() failed: %s\n", curl_easy_strerror(res));
    }
    curl_easy_cleanup(curl_handle);
    return 0;
}

int main(void) {
    const char* url = "https://leetcode.com/problems/valid-parentheses/description/";
    struct memory* buffer = calloc(1, sizeof(struct memory));
    buffer->response = calloc(1, 4096);
    if (fetch(url, buffer) == 0) {
        printf("%s\n", buffer->response);
    }
    free(buffer->response);
    free(buffer);
    return 0;
}
