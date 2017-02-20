#pragma once

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
char* DownloadString(char *url);
