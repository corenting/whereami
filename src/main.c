/*
whereami, a console application to get your location using freegeoip.net
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
#include "jsmn.h"
  
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
  strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
    return 0;
	}
  return -1;
}

struct Position {
   char *city;
   char *region;
   char *country;
   char *latitude;
   char *longitude;
};

int main(void) {
  //Get the string
  const char * webString = DownloadString("http://freegeoip.net/json/");
  
  //Check for errors
  if(!webString|| strlen(webString) == 0) {
    printf("freegeoip.net data error\n");
    return 1;
  }
  
  //Parse JSON
  int r;
  jsmn_parser p;
  jsmntok_t t[128];
  jsmn_init(&p);
  r = jsmn_parse(&p, webString, strlen(webString), t, sizeof(t)/sizeof(t[0]));
  if (r < 0) {
    printf("JSON parsing error (%d)\n", r);
    return 1;
  }
  
  //Test top-level element is an object
  if (r < 1 || t[0].type != JSMN_OBJECT) {
    printf("JSON parsing error (object expected)\n");
    return 1;
  }
  
  //Parse elements
  struct Position pos;
  
  for (int i = 1; i < r; i++) {
    if (jsoneq(webString, &t[i], "city") == 0) {
      asprintf(&pos.city, "%.*s\n", t[i+1].end-t[i+1].start, webString + t[i+1].start);
			i++;
    }
    else if (jsoneq(webString, &t[i], "region_name") == 0) {
      asprintf(&pos.region, "%.*s\n", t[i+1].end-t[i+1].start, webString + t[i+1].start);
			i++;
    }
    else if (jsoneq(webString, &t[i], "country_name") == 0) {
      asprintf(&pos.country, "%.*s\n", t[i+1].end-t[i+1].start, webString + t[i+1].start);
			i++;
    }
    else if (jsoneq(webString, &t[i], "latitude") == 0) {
      asprintf(&pos.latitude, "%.*s\n", t[i+1].end-t[i+1].start, webString + t[i+1].start);
			i++;
    }
    else if (jsoneq(webString, &t[i], "longitude") == 0) {
      asprintf(&pos.longitude, "%.*s\n", t[i+1].end-t[i+1].start, webString + t[i+1].start);
			i++;
    }
  }
  
  printf("%s",pos.region);
  
  //TODO : free() things
  if(pos.city) free(pos.city);
  if(pos.region) free(pos.region);
  if(pos.country) free(pos.country);
  if(pos.latitude) free(pos.latitude);
  if(pos.longitude) free(pos.longitude);

  return 0;
}