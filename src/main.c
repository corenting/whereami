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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include <curl/curl.h>

#include "download.h"
 
int main(void) {

  //Get the string
  const char * webString = DownloadString("http://freegeoip.net/json/");
  
  printf("%s\n", webString);

  return 0;
}