/*
whereami, a console application to get your location using geoip.nekudo.com
Copyright (C) 2016  corenting

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
    printf("whereami - get your location using geoip.nekudo.com\n\n"
           "Usage :\n"
           "-c or --city : print only the city\n"
           "-co or --country : print only the country\n"
           "-p or --position : print only the position\n");
    return 0;
  }

  //Get the string
  char *webString = DownloadString("http://geoip.nekudo.com/api/json");

  //Check for errors
  if(!webString|| strlen(webString) == 0) {
    printf("geoip.nekudo.com data error\n");
    return 1;
  }

  //Parse JSON
  cJSON *json = cJSON_Parse(webString);
  cJSON *location = cJSON_GetObjectItem(json,"location");

  //Parse elements and check if we have a city
  struct Position pos;
  char *city = cJSON_GetObjectItem(json,"city")->valuestring;
  if (!city)
    asprintf(&city, "Unknown city");
  asprintf(&pos.city,city);
  free(city);
  asprintf(&pos.country,cJSON_GetObjectItem(cJSON_GetObjectItem(json,"country"),"name")->valuestring);
  pos.latitude = cJSON_GetObjectItem(location,"latitude")->valuedouble;
  pos.longitude = cJSON_GetObjectItem(location,"longitude")->valuedouble;


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

  if(pos.city) free(pos.city);
  if(pos.country) free(pos.country);
  free(webString);
  cJSON_Delete(json);
  return 0;
}
