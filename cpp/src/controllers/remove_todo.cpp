#include "../../include/controllers.h"
#include "../../include/initialize_db.h"
#include <crow/logging.h>
#include <sqlite3.h>

int remove_todo(Config config, int id) {
  sqlite3 *db;
  int rc = sqlite3_open(config.database.c_str(), &db);

  sqliteResult(db, rc, nullptr, [&]() {
    const char *getTodo = "DELETE FROM todo WHERE id = ?;";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, getTodo, -1, &stmt, nullptr);

    sqliteResult(db, rc, nullptr, [&]() {
      sqlite3_bind_int(stmt, 1, id);

      rc = sqlite3_step(stmt);
      sqliteResult(db, rc, "Successfully removed todo",
                   [&]() { sqlite3_finalize(stmt); });
    });
  });

  sqlite3_close(db);
  return 0;
}
