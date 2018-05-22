#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MELOETTA 648
#define ALL_POKES 1000

#include "stat.h"
#include "nature.h"

typedef struct pokemon {
	int species;
	unsigned char level;
	unsigned short nature;
	unsigned char ivs[6];
	unsigned char evs[6];
} pokemon;

typedef struct pokeinfo {
	int species;
	unsigned char stats[6];
} pokeinfo;
pokeinfo pokinf[ALL_POKES];

int natureBoost(int nat, int statID) {
	if (statID == 0) {
		return 0;
	}
	statID--;
	return (nat >> statID) & 1;
}
int natureHinder(int nat, int statID) {
	return natureBoost(nat >> 5, statID);
}
float natureFactor(int nat, int statID) {
	int up = natureBoost(nat, statID);
	int down = natureHinder(nat, statID);
	if (up == down) {
		return 1;
	} else if (up) {
		return 1.1;
	} else {
		return .9;
	}
}
int calcStat(struct pokemon* poke, int statID) {
	int spec = poke->species;
	int base = pokinf[spec].stats[statID];
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

void init() {
	pokeinfo* p = &pokinf[MELOETTA];
	p->stats[0] = 100;
	p->stats[1] = 77;
	p->stats[2] = 77;
	p->stats[3] = 128;
	p->stats[4] = 128;
	p->stats[5] = 90;
}
int bruteForceIV(struct pokemon* poke, int statID, int realStat, int* minIV, int* maxIV) {
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
	} while (read != -1);
	//TODO error
	return 0;
}
int main() {
	init();
	pokemon Meloetta;
	memset(&Meloetta, 0, sizeof(struct pokemon));
	Meloetta.species = MELOETTA;
	Meloetta.level = 50;
	Meloetta.nature = promptNature();
	int stats[6];
	for (int statID = 0; statID < 6; statID++) {
		printf("%s: ", STAT_STRINGS[statID]);
		scanf("%d", stats+statID);
	}
	printf("\n");
	for (int statID = 0; statID < 6; statID++) {
		int minIV;
		int maxIV;
		bruteForceIV(&Meloetta, statID, stats[statID], &minIV, &maxIV);
		printf("%s: %d", STAT_STRINGS[statID], minIV);
		if (minIV != maxIV) {
			printf("-%d", maxIV);
		}
		printf("\n");
	}
}
