/*
whereami, a console application to get your location using ipapi.co
Copyright (C) 2017  corenting

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#include "download.h"
#include "cJSON.h"

struct Position {
   char *city;
   char *country;
   double latitude;
   double longitude;
};

int main(int argc, const char* argv[]) {

  //Help
  if(argc > 1 && (!strncmp(argv[1], "-h",2) || !strncmp(argv[1], "--help",6))) {
    printf("whereami - get your location using ipapi.co\n\n"
           "Usage :\n"
           "-c or --city : print only the city\n"
           "-co or --country : print only the country\n"
           "-p or --position : print only the position\n");
    return 0;
  }

  // Try to get from ipapi.co
  char *webString = DownloadString("https://ipapi.co/json");

  //Check for errors
  if(webString == NULL || strlen(webString) == 0) {
    fprintf(stderr, "Cannot download position informations\n");
    return 1;
  }

  //Parse JSON
  cJSON *json = cJSON_Parse(webString);
  const char *errorPtr = cJSON_GetErrorPtr();
  if (errorPtr != NULL)
  {
    fprintf(stderr, "Error while parsing JSON from API\n");
    return 1;
  }

  //Parse elements and check if we have a city
  struct Position pos;
  int ret = asprintf(&pos.city,"%s",cJSON_GetObjectItem(json,"city")->valuestring);
  if (ret == -1) {
    return 1;
  }
  ret = asprintf(&pos.country,"%s",cJSON_GetObjectItem(json,"country_name")->valuestring);
  if (ret == -1) {
    return 1;
  }
  pos.latitude = cJSON_GetObjectItem(json,"latitude")->valuedouble;
  pos.longitude = cJSON_GetObjectItem(json,"longitude")->valuedouble;


  //Display according to the argument provided
  if(argc > 1) {
    if(!strncmp(argv[1], "-co",3) || !strncmp(argv[1], "--country",9)) {
      printf("%s\n",pos.country);
    }
    else if(!strncmp(argv[1], "-c",2) || !strncmp(argv[1], "--city",6)) {
      printf("%s\n",pos.city);
    }
    else if(!strncmp(argv[1], "-p",2) || !strncmp(argv[1], "--position",10)) {
      printf("%lf,%lf\n",pos.latitude,pos.longitude);
    }
    else {
      printf("Unknown option '%s'\n",argv[1]);
    }
  }
  else {
    printf("%s, %s (%lf,%lf)\n",pos.city, pos.country,pos.latitude,pos.longitude);
  }

  // Free and return
  free(pos.city);
  free(pos.country);
  free(webString);
  cJSON_Delete(json);
  return 0;
}