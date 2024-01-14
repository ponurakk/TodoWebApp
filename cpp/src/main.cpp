#include "../include/config.h"
#include "../include/controllers.h"
#include "../include/initialize_db.h"
#include "../include/logger.h"
#include <crow/app.h>
#include <crow/common.h>
#include <crow/http_response.h>
#include <crow/json.h>
#include <crow/logging.h>
#include <fstream>
#include <json/json.h>
#include <json/value.h>
#include <optional>

using crow::json::wvalue, crow::mustache::context, crow::mustache::load,
    crow::logger, crow::HTTPMethod;
using std::string, std::ifstream, std::nullopt, std::exception, std::stoi;

Config::operator wvalue() const {
  return {
      {"addr", this->addr}, {"port", this->port}, {"database", this->database}};
}

Config get_config() {
  ifstream file("config.json");
  Json::Value json_config;
  file >> json_config;
  Config config{json_config["addr"].as<string>(), json_config["port"].as<int>(),
                json_config["database"].as<string>()};
  return config;
}

optional<SortDirection> TodoQuery::parseSortDirection(const string &str) {
  if (str == "asc") {
    return SortDirection::ASC;
  } else if (str == "desc") {
    return SortDirection::DESC;
  } else {
    return nullopt; // Invalid value
  }
}

TodoQuery::TodoQuery(const crow::query_string &qs) {
  char *sortValue = qs.get("sort");
  if (sortValue != nullptr) {
    this->sort = parseSortDirection(sortValue);
  } else {
    this->sort = SortDirection::ASC;
  }

  const char *pageLimitValue = qs.get("page_limit");
  try {
    this->page_limit = (pageLimitValue != nullptr && *pageLimitValue != '\0')
                           ? stoi(pageLimitValue)
                           : 10;
  } catch (const exception &e) {
    // Handle conversion error
    CROW_LOG_ERROR << "Error converting page_limit: " << e.what();
    this->page_limit = 10;
  }
}

TodoQuery::operator wvalue() const {
  return {{"sort", this->sort.value_or(SortDirection::ASC)},
          {"page_limit", this->page_limit.value_or(10)}};
}

int main() {
  CustomLogger logger;
  logger::setHandler(&logger);
  crow::SimpleApp app;
  app.loglevel(LogLevel::INFO);

  Config config = get_config();

  createDb(config);

  CROW_ROUTE(app, "/")
  ([&config](const crow::request &req) {
    auto page = load("index.mustache");
    context ctx;

    TodoQuery query = req.url_params;
    vector<Todo> todos = list_todo(config);

    ctx["todos"] = todos;
    ctx["config"] = config;
    ctx["query"] = query;
    return page.render(ctx);
  });

  CROW_ROUTE(app, "/api/todo")
      .methods(HTTPMethod::POST)(
          [&config](const crow::request &req, crow::response &res) {
            crow::query_string params("?" + req.body);
            Todo td = params;
            add_todo(td, config);
            res.redirect("/");
            res.end();
          });

  CROW_ROUTE(app, "/api/todo/<int>")
  ([&config](crow::response &res, int id) {
    Todo todo = get_todo(config, id);
    res.end(wvalue(todo).dump());
  });

  CROW_ROUTE(app, "/api/todo/<int>")
      .methods(HTTPMethod::PATCH)(
          [&config](crow::request &req, crow::response &res, int id) {
            crow::query_string params("?" + req.body);

            Json::CharReaderBuilder reader;
            Json::Value root;
            std::istringstream jsonStream(req.body);
            Json::parseFromStream(reader, jsonStream, &root, nullptr);

            Todo td = root;
            update_todo(td, config, id);
            res.end();
          });

  CROW_ROUTE(app, "/api/todo/<int>")
      .methods(HTTPMethod::DELETE)([&config](crow::response &res, int id) {
        remove_todo(config, id);
        res.end();
      });

  CROW_ROUTE(app, "/api/todos")
  ([&config](const crow::request &req, crow::response &res) {
    vector<Todo> todos = list_todo(config);
    wvalue wv;

    for (size_t i = 0; i < todos.size(); i++) {
      wv[i] = todos.at(i);
    }
    res.end(wv.dump());
  });

  app.port(3000).multithreaded().run();
}
