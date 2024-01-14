#include "../../include/controllers.h"
#include "../../include/initialize_db.h"
#include <crow/logging.h>
#include <sqlite3.h>

Todo get_todo(Config config, int id) {
  sqlite3 *db;
  int rc = sqlite3_open(config.database.c_str(), &db);

  Todo todo;

  sqliteResult(db, rc, nullptr, [&]() {
    const char *getTodo = "SELECT * FROM todo WHERE id = ? LIMIT 1";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, getTodo, -1, &stmt, nullptr);

    sqliteResult(db, rc, nullptr, [&]() {
      sqlite3_bind_int(stmt, 1, id);

      rc = sqlite3_step(stmt);
      sqliteResult(db, rc, "Selected todo", [&]() {
        int id = sqlite3_column_int(stmt, 0);
        const char *title =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        const char *text =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        bool is_done = sqlite3_column_int(stmt, 3);
        const char *created_at =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        const char *updated_at =
            reinterpret_cast<const char *>(sqlite3_column_text(stmt, 5));
        todo = Todo(id, title, text, is_done, created_at, updated_at);
        sqlite3_finalize(stmt);
      });
    });
  });

  sqlite3_close(db);
  return todo;
}
