#include "../include/config.h"
#include <sqlite3.h>

int sqliteResult(sqlite3 *db, int rc, const char *successMessage,
                 std::function<void()> onSuccess) {
  if (rc == SQLITE_OK || rc == SQLITE_DONE || rc == SQLITE_ROW) {
    if (successMessage != nullptr) {
      CROW_LOG_INFO << successMessage;
    }
    if (onSuccess) {
      onSuccess();
    }
  } else {
    CROW_LOG_ERROR << "SQL error: " << sqlite3_errmsg(db);
  }
  return rc;
}

int createDb(Config config) {
  sqlite3 *db;
  int rc = sqlite3_open(config.database.c_str(), &db);

  sqliteResult(db, rc, "Opened database successfully", [&]() {
    const char *createTable =
        "CREATE TABLE IF NOT EXISTS \"todo\" ("
        "\"id\" INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
        "\"title\" text NOT NULL, "
        "\"text\" text NOT NULL,"
        "\"is_done\" boolean NOT NULL DEFAULT FALSE,"
        "\"created_at\" text NOT NULL,"
        "\"updated_at\" text NOT NULL"
        ");";

    rc = sqlite3_exec(db, createTable, 0, 0, 0);

    sqliteResult(db, rc, "Table created successfully", nullptr);
  });

  sqlite3_close(db);
  return 0;
}
