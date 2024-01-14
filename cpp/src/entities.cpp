#include "../include/entities.h"
#include <crow/query_string.h>
#include <json/value.h>
#include <string>

using crow::json::wvalue;
using std::stoi;

Todo::operator wvalue() const {
  return {{"id", this->id},
          {"title", this->title},
          {"text", this->text},
          {"is_done", this->is_done},
          {"created_at", this->created_at},
          {"updated_at", this->updated_at}};
}

Todo::Todo() {}

Todo::Todo(int &id, const char *&title, const char *&text, bool &is_done,
           const char *&created_at, const char *&updated_at)
    : id(id), title(title), text(text), is_done(is_done),
      created_at(created_at), updated_at(updated_at) {}

Todo::Todo(const crow::query_string &qs) {
  this->id = stoi(qs.get("id"));
  this->title = qs.get("title");
  this->text = qs.get("text");
  this->is_done = qs.get("is_done");
  this->created_at = qs.get("created_at");
  this->updated_at = qs.get("updated_at");
}

Todo::Todo(const Json::Value &qs) {
  this->id = qs["id"].asInt();
  this->title = qs["title"].asString();
  this->text = qs["text"].asString();
  this->is_done = qs["is_done"].asBool();
  this->created_at = qs["created_at"].asString();
  this->updated_at = qs["updated_at"].asString();
}
