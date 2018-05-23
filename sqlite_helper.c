#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "stat.h"
#include "structs.h"
static sqlite3 *db;
void init_db() {
	int rc = sqlite3_open("poke.db", &db);
	//TODO check this error
}
struct callback_data {
	int valid;
	pokeinfo* pkinf;
};
int callback(void* vp, int argc, char **argv, char** azColName) {
	struct callback_data* cb = vp;
	pokeinfo* pkinf = cb->pkinf;
	cb->valid = 1;
	for (int i = 0; i < argc; i++) {
		if (strcmp(azColName[i], "nid") == 0) {
			pkinf->nid = atoi(argv[i]);
		}
		for (int s = 0; s < NUM_STATS; s++) {
			if (strcmp(STAT_STRINGS[s], azColName[i]) == 0) {
				pkinf->stats[s] = atoi(argv[i]);
				break;
			}
		}
	}
	return 0;
}
pokeinfo* getInfBySQL(char* sql) {
	struct callback_data cb;
	memset(&cb, 0, sizeof(cb));
	cb.valid = 0;
	cb.pkinf = malloc(sizeof(pokeinfo));
	char* err_msg = NULL;

	int rc = sqlite3_exec(db, sql, callback, &cb, &err_msg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL ERROR: %d:%s\n", rc, err_msg);
		sqlite3_free(err_msg);
		return NULL;
	}
	if (!cb.valid) {
		return NULL;
	}
	return cb.pkinf;
}
pokeinfo* getInfByNID(int nid) {
	const int BUFF_SZ = 2048;
	char sql[BUFF_SZ];
	snprintf(sql,  BUFF_SZ, "select * from pokemon where nid = %d", nid);
	return getInfBySQL(sql);
}
pokeinfo* getInfByName(char* name) {
	char* sanitize = strdup(name);
	for (int i = 0; name[i]; i++) {
		if ('A' <= sanitize[i] && sanitize[i] <= 'Z') {
			sanitize[i] ^= ' ';
			continue;
		}
		if (sanitize[i] == '\n') {
			sanitize[i] = 0;
			break;
		}
		//if it's not alpha-numeric set it to a hyphen
		//[-a-z0-9] are all valid characters at this point
		if ('a' <= sanitize[i] && sanitize[i] <= 'z') {
		} else if ('0' <= sanitize[i] && sanitize[i] <= '9') {
		} else {
			sanitize[i] = '-';
		}
	}
	const int BUFF_SZ = 2048;
	char sql[BUFF_SZ];
	snprintf(sql,  BUFF_SZ, "select * from pokemon where name = '%s'", sanitize);
	return getInfBySQL(sql);
}
