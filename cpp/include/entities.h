#ifndef ENTITIES_H_
#define ENTITIES_H_

#include <crow/json.h>
#include <crow/query_string.h>
#include <json/value.h>
#include <string>

using std::string;

struct Todo {
  int id;
  string title;
  string text;
  bool is_done;
  string created_at;
  string updated_at;

  operator crow::json::wvalue() const;
  Todo();
  Todo(const crow::query_string &qs);
  Todo(const Json::Value &qs);
  Todo(int &, const char *&, const char *&, bool &, const char *&,
       const char *&);
};

#endif
