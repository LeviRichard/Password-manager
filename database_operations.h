#ifndef DATABASE_OPERATIONS_H
#define DATABASE_OPERATIONS_H


#define DB_FILE "vault.json"
extern char hashed_master_password[32];
#include <json-c/json.h>

int db_add_entry(char *username, char *website, char *password);

int db_read_all();

int db_check_password(char *hashed_master);

void delete_db();

int database_exists();
#endif // !DATABASE_OPERATIONS_H

