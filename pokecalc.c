#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MELOETTA 648

#include "stat.h"
#include "nature.h"
#include "structs.h"
#include "sqlite_helper.h"
#include "tidy.h"

int calcStat(struct pokemon* poke, int statID) {
	int nid = poke->nid;
	pokeinfo* pkinf = getInfByNID(nid);
	if (pkinf == NULL) {
		fprintf(stderr, "ERROR: Invalid species!\n");
		return -1;
	}
	int formID = poke->formID;
	int base = pkinf->forms[formID].stats[statID];
	int iv = poke->ivs[statID];
	int ev = poke->evs[statID];
	int level = poke->level;
	float nature = natureFactor(poke->nature, statID);
	int stat = 0;
	if (statID == Hp) {
		stat = ((2*base+iv+(ev/4))*level)/100 + level + 10;
	} else {
		stat = ((2*base+iv+(ev/4))*level)/100 + 5;
		float fstat = stat*nature;
		stat = (int) fstat;
	}
	return stat;
}

void bruteForceIV(struct pokemon* poke, int statID, int realStat, int* minIV, int* maxIV) {
	int loc = -1;
	*minIV = -1;
	*maxIV = -1;
	for (int i = 0; i < 32; i++) {
		poke->ivs[statID] = i;
		int stat = calcStat(poke, statID);
		if (stat == realStat) {
			if (loc == -1) {
				loc = 0;
				*minIV = i;
				*maxIV = i;
			} else if (loc == 0) {
				*maxIV = i;
			}
		} else if (loc == 0) {
			break;
		}
	}
}
int promptNature() {
	int read;
	do {
		printf("Nature: ");
		char *line = NULL;
		size_t sz;
		read = getline(&line, &sz, stdin);
		for (int n = 0; n < NUM_NATS; n++) {
			const char* nat = NAT_STRINGS[n];
			int match = 1;
			for (int i = 0; i < sz && nat[i]; i++) {
				if ((nat[i] | ' ') != (line[i] | ' ')) {
					match = 0;
					break;
				}
			}
			if (match) {
				return NAT_MAP[n];
			}
		}
		//free line?
	} while (read != -1);
	//TODO error
	return 0;
}
int promptPokemon() {
	int read;
	do {
		printf("Pokemon: ");
		char *line = NULL;
		size_t sz;
		read = getline(&line, &sz, stdin);
		pokeinfo *pkinf = getInfByName(line);
		if (pkinf != NULL) {
			int ret = pkinf->nid;
			free(pkinf);
			return ret;
		}
		//free line?
	} while (read != -1);
	//TODO error
	return 0;
}
int main() {
	init_db();
	pokemon poke;
	memset(&poke, 0, sizeof(struct pokemon));
	poke.nid = promptPokemon();
	poke.nature = promptNature();
	poke.level = 0;
	int read = 0;
	printf("Level: ");
	scanf("%d", &read);
	poke.level = read;
	int stats[6];
	for (int statID = 0; statID < 6; statID++) {
		printf("%s: ", STAT_STRINGS[statID]);
		scanf("%d", stats+statID);
	}
	printf("\n");
	for (int statID = 0; statID < 6; statID++) {
		int minIV;
		int maxIV;
		bruteForceIV(&poke, statID, stats[statID], &minIV, &maxIV);
		printf("%s: %d", STAT_STRINGS[statID], minIV);
		if (minIV != maxIV) {
			printf("-%d", maxIV);
		}
		printf("\n");
	}
	close_db(0);
}
