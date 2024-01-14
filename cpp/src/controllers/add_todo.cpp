#include "../../include/controllers.h"
#include "../../include/initialize_db.h"
#include "../../include/logger.h"
#include <sqlite3.h>

int add_todo(Todo td, Config config) {
  sqlite3 *db;
  int rc = sqlite3_open(config.database.c_str(), &db);

  sqliteResult(db, rc, nullptr, [&]() {
    const char *addTodo =
        "INSERT INTO"
        "\"todo\"(\"title\",\"text\",\"is_done\",\"created_at\",\"updated_at\")"
        "VALUES (?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, addTodo, -1, &stmt, nullptr);

    sqliteResult(db, rc, nullptr, [&]() {
      sqlite3_bind_text(stmt, 1, td.title.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_text(stmt, 2, td.text.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_int(stmt, 3, false);
      sqlite3_bind_text(stmt, 4, getCurrentTime("%Y-%m-%d %H:%M:%S").c_str(),
                        -1, SQLITE_STATIC);
      sqlite3_bind_text(stmt, 5, getCurrentTime("%Y-%m-%d %H:%M:%S").c_str(),
                        -1, SQLITE_STATIC);

      rc = sqlite3_step(stmt);
      sqliteResult(db, rc, "Successfully added todo",
                   [&]() { sqlite3_finalize(stmt); });
    });
  });

  sqlite3_close(db);
  return 0;
}
