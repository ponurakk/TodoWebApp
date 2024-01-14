#include "../../include/controllers.h"
#include "../../include/initialize_db.h"
#include <crow/logging.h>
#include <sqlite3.h>
#include <string>
#include <vector>

using std::vector, std::stoi;

struct ResultData {
  vector<Todo> objects;
};

int callback(void *data, int argc, char **argv, char **colNames) {
  ResultData *resultData = static_cast<ResultData *>(data);

  int id = stoi(argv[0]);
  const char *title = argv[1];
  const char *text = argv[2];
  bool is_done = argv[3];
  const char *created_at = argv[4];
  const char *updated_at = argv[5];

  resultData->objects.emplace_back(id, title, text, is_done, created_at,
                                   updated_at);

  return 0;
}

// TODO: Make sorting and limit
vector<Todo> list_todo(Config config) {
  sqlite3 *db;
  int rc = sqlite3_open(config.database.c_str(), &db);

  ResultData resultData;

  sqliteResult(db, rc, nullptr, [&]() {
    const char *getTodo = "SELECT * FROM todo";
    rc = sqlite3_exec(db, getTodo, callback, &resultData, 0);

    sqliteResult(db, rc, "Selected todos", nullptr);
  });

  sqlite3_close(db);
  return resultData.objects;
}
