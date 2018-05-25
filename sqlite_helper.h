#include "structs.h"
void init_db();
void close_db(int);
pokeinfo* getInfByNID(int);
pokeinfo* getInfByName(char*);
int sql_exec(char*);
