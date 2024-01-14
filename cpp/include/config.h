#ifndef CONFIG_H_
#define CONFIG_H_

#include <crow/json.h>
#include <string>

using std::string;

struct Config {
  string addr;
  int port;
  string database;

  operator crow::json::wvalue() const;
};

Config get_config();

#endif
