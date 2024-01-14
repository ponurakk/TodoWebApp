#ifndef INITIALIZE_DB_H_
#define INITIALIZE_DB_H_

#include "config.h"
#include <sqlite3.h>

int createDb(Config config);
int sqliteResult(sqlite3 *db, int rc, const char *successMessage,
                 std::function<void()> onSuccess);
#endif
