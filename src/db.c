#include "db.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool db_connect(Database *db, const char *conninfo) {
    db->conn = PQconnectdb(conninfo);
    if (PQstatus(db->conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(db->conn));
        return false;
    }

    return true;
}

PGresult* db_execute(Database *db, const char *query) {
    if (!db || !db->conn) {
        fprintf(stderr, "Database connection is not established.\n");
        return NULL;
    }

    PGresult *res = PQexec(db->conn, query);
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        fprintf(stderr, "Query execution failed: %s", PQerrorMessage(db->conn));
        PQclear(res);
        return NULL;
    }

    return res;
}

void db_disconnect(Database *db) {
    if (db && db->conn) {
        PQfinish(db->conn);
        db->conn = NULL;
    }
}