#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct pokemon {
	int nid;
	unsigned char level;
	unsigned short nature;
	unsigned char ivs[6];
	unsigned char evs[6];
} pokemon;

typedef struct pokeinfo {
	int nid;
	unsigned char stats[6];
} pokeinfo;

#endif
