#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "stat.h"
#include "structs.h"
int createDB() {
	sqlite3 *db;
	int rc;

	rc = sqlite3_open("test.db", &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return 1;
	}
	//TODO stub, actually populate creat & populate tables
	sqlite3_close(db);
	return 0;
}
int callback(void* vp, int argc, char **argv, char** azColName) {
	pokeinfo* pkinf = vp;
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
pokeinfo* getInfByNID(int nid) {
	const int BUFF_SZ = 2048;
	pokeinfo* pkinf = malloc(sizeof(pokeinfo));
	sqlite3 *db;
	char* err_msg = NULL;
	int rc = sqlite3_open("test.db", &db);

	char sql[BUFF_SZ];
	snprintf(sql,  BUFF_SZ, "select * from pokemon where nid = %d", nid);
	rc = sqlite3_exec(db, sql, callback, pkinf, &err_msg);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL ERROR: %d:%s\n", rc, err_msg);
		sqlite3_free(err_msg);
		return NULL;
	}
	return pkinf;
}
