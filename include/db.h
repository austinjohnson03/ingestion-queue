#ifndef DB_H
#define DB_H

#include <libpq-fe.h>
#include <stdbool.h>

typedef struct {
    PGconn *conn;
} Database;

bool db_connect(Database *db, const char *conninfo);

PGresult* db_execute(Database *db, const char *query);

void db_disconnect(Database *db);

#endif // DB_H