#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h> 

#include "download.h"

struct MemoryStruct {
  char *memory;
  size_t size;
};

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *) userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  if(mem->memory == NULL) {
    printf("Memory error (realloc)\n");
    return 0;
  }
 
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
  return realsize;
}

const char* DownloadString(char *url) {
  //Chunk
  struct MemoryStruct chunk;
  chunk.memory = malloc(1);
  chunk.size = 0;
  
  //Curl init
  CURL *curl_handle;
  CURLcode res;
  curl_global_init(CURL_GLOBAL_ALL);
  curl_handle = curl_easy_init(); //session init
  curl_easy_setopt(curl_handle, CURLOPT_URL, url);
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback); //send data to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk); //pass struct to the callback function
  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "whereami");
 
  res = curl_easy_perform(curl_handle); //Download
 
  //Check for error
  if(res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
  }
 
  //Cleanup
  free(chunk.memory);
  curl_easy_cleanup(curl_handle);
  curl_global_cleanup();
  
  return chunk.memory;
}