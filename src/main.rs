extern crate hyper;
extern crate rustc_serialize;
extern crate getopts;

use std::io::Read;
use std::process;
use std::env;

use hyper::Client;
use hyper::status::StatusCode;
use hyper::header::Connection;
use rustc_serialize::json::Json;
use getopts::Options;

fn print_usage(program: &str, opts: Options) {
  let brief = format!("Usage: {} FILE [options]", program);
  print!("{}", opts.usage(&brief));
}

fn main() {

  //Arguments
  let args: Vec<String> = env::args().collect();
  let program = args[0].clone();

  let mut opts = Options::new();
  opts.optflag("h", "help", "print this help menu");
  opts.optflag("c", "city", "print only the city");
  opts.optflag("p", "position", "print only the position");
  let matches = match opts.parse(&args[1..]) {
    Ok(m) => { m }
    Err(f) => { panic!(f.to_string()) }
  };

  if matches.opt_present("h") {
    print_usage(&program, opts);
    return;
  }

  //Download
  let client = Client::new();
  let mut res = client.get("http://freegeoip.net/json/")
  .header(Connection::close())
  .send().unwrap();
  
  if res.status != StatusCode::Ok {
    process::exit(1);
  }

  //Response
  let mut body = String::new();
  res.read_to_string(&mut body).unwrap();

  //Json
  let json = Json::from_str(&body).unwrap();
  let json_object = json.as_object().unwrap();
  
  //Vars
  let city = json_object.get("city").unwrap().as_string().unwrap();;
  let region = json_object.get("region_name").unwrap().as_string().unwrap();;
  let country = json_object.get("country_name").unwrap().as_string().unwrap();
  let latitude = json_object.get("latitude").unwrap();
  let longitude = json_object.get("longitude").unwrap();

  if matches.opt_present("p") {
    println!("{},{}",latitude,longitude);
  }
  else if matches.opt_present("c") {
    println!("{}",city);
  }
  else {
    println!("{}, {}, {} ({},{})", city,region,country,latitude,longitude);
  }
}