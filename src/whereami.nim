let doc = """
whereami - get your location using ipapi.co

Usage:
  whereami [--filter=<field_name>]
  whereami (-h | --help)
  whereami (-v | --version)

Options:
  --filter=<field_name>   Show only the specified information (city, country or position)
  -h --help               Show this screen.
  -v --version            Show application version
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

proc getCurrentPosition(): Position =
  let client = newHttpClient()
  let apiResponse = client.get(constants.apiEndpoint)

  if apiResponse.code() != HttpCode(200):
    raise newException(IOError, "Request error")

  try:
    var currentPosition = Position()
    let jsonResponse = parseJson(apiResponse.body)
    currentPosition = to(jsonResponse, Position)
    return currentPosition
  except:
    raise newException(ValueError, "Error while parsing JSON from API")

proc getPositionString(pos: Position, args: Table[string, Value]): string =
  let filter = $args["--filter"]
  if filter == "city":
    return pos.city
  elif filter == "country":
    return pos.country_name
  elif filter == "position":
    return $pos.latitude & "," & $pos.longitude
  else:
    return $pos.city & ", " & $pos.country_name & " (" & $pos.latitude & "," & $pos.longitude & ")"

proc main() =
  let args = docopt(doc, version = "whereami 3.0")

  var currentPos = Position()
  try:
    currentPos = getCurrentPosition()
  except IOError:
    stderr.writeLine("Cannot download position informations")
    quit(1)
  except ValueError:
    stderr.writeLine("Error while parsing JSON from API")
    quit(1)

  echo getPositionString(currentPos, args)

when isMainModule:
  main()
