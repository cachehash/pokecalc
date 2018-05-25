#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct pokemon {
	int nid;
	unsigned char formID;
	unsigned char level;
	unsigned short nature;
	unsigned char ivs[6];
	unsigned char evs[6];
} pokemon;

typedef struct pokeform {
	unsigned char stats[6];
} pokeform;
typedef struct pokeinfo {
	int nid;
	int numForms;
	pokeform* forms;
} pokeinfo;

#endif
