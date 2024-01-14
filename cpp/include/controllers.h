#ifndef CONTROLLERS_H_
#define CONTROLLERS_H_

#include "../include/entities.h"
#include "config.h"
#include <crow/json.h>
#include <crow/query_string.h>
#include <optional>
#include <string>
#include <vector>

using std::optional, std::string, std::vector;

enum SortDirection { ASC, DESC };

struct TodoQuery {
  optional<SortDirection> sort;
  optional<int> page_limit;

  operator crow::json::wvalue() const;
  TodoQuery(const crow::query_string &qs);

private:
  static optional<SortDirection> parseSortDirection(const string &str);
};

int add_todo(Todo td, Config cnf);
Todo get_todo(Config cnf, int id);
vector<Todo> list_todo(Config cnf);
int remove_todo(Config cnf, int id);
int update_todo(Todo td, Config cnf, int id);

#endif
