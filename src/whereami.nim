let doc = """
whereami - get your location using ipapi.co

Usage:
  whereami [-c | --city] [-co | --country] [-p | --position]
  whereami (-h | --help)
  whereami --version

Options:
  -h --help      Show this screen.
  -c --city      Show only the city
  -co --country  Show only the country
  -p --position  Show only the position
"""

import constants

import docopt
import httpClient
import json

type
  Position = object
    city*: string
    country_name*: string
    latitude*: float
    longitude*: float


let args = docopt(doc, version = "whereami 3.0")

let client = newHttpClient()
let apiResponse = client.get(constants.apiEndpoint)

if apiResponse.code() != HttpCode(200):
  stderr.writeLine("Cannot download position informations")
  quit(1)

var currentPosition = Position()
try:
  let jsonResponse = parseJson(apiResponse.body)
  currentPosition = to(jsonResponse, Position)
except:
  stderr.writeLine("Error while parsing JSON from API")
  quit(1)

if args["--city"]:
  echo(currentPosition.city)
elif args["--country"]:
  echo(currentPosition.country_name)
elif args["--position"]:
  echo($currentPosition.latitude & "," & $currentPosition.longitude)
else:
  echo(
    $currentPosition.city & ", " &
    $currentPosition.country_name & " (" &
    $currentPosition.latitude & "," &
    $currentPosition.longitude & ")"
  )
