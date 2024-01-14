#include "../../include/controllers.h"
#include "../../include/initialize_db.h"
#include "../../include/logger.h"
#include <crow/logging.h>
#include <sqlite3.h>

int update_todo(Todo td, Config config, int id) {
  sqlite3 *db;
  int rc = sqlite3_open(config.database.c_str(), &db);

  sqliteResult(db, rc, nullptr, [&]() {
    const char *getTodo = "UPDATE \"todo\" SET \"title\" = ?, \"text\" = ?, "
                          "\"is_done\" = ?, \"updated_at\" = ? WHERE id = ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, getTodo, -1, &stmt, nullptr);

    sqliteResult(db, rc, nullptr, [&]() {
      sqlite3_bind_text(stmt, 1, td.title.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_text(stmt, 2, td.text.c_str(), -1, SQLITE_STATIC);
      sqlite3_bind_int(stmt, 3, td.is_done ? 1 : 0);
      // TODO: Fix date instead of blob
      sqlite3_bind_text(stmt, 4, getCurrentTime("%H:%M:%S").c_str(), -1,
                        SQLITE_STATIC);
      sqlite3_bind_int(stmt, 5, id);

      rc = sqlite3_step(stmt);
      sqliteResult(db, rc, "Successfully updated todo",
                   [&]() { sqlite3_finalize(stmt); });
    });
  });

  sqlite3_close(db);
  return 0;
}
