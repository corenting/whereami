#pragma once

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
const char* DownloadString(char *url);