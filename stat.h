#ifndef STAT_H
#define STAT_H

#define FOREACH_STAT(D) \
D(Hp, "HP") \
D(Atk, "Attack") \
D(Def, "Defense") \
D(SpA, "Sp. Atk") \
D(SpD, "Sp. Def") \
D(Spe, "Speed") \

#pragma GCC diagnostic ignored "-Wunused-variable"

#define GENERATE_ENUM(TOK, STR) TOK,
enum STAT_ENUM {
	FOREACH_STAT(GENERATE_ENUM)
};

#define GENERATE_STRING(TOK, STR) #TOK,
static const char *STAT_STRINGS[] = {
	FOREACH_STAT(GENERATE_STRING)
};

#define GENERATE_LONG_NAMES(TOK, STR) STR,
static const char *STAT_LONG_NAMES[] = {
	FOREACH_STAT(GENERATE_LONG_NAMES)
};

#define COUNT_STATS(TOK, STR) +1
static const int NUM_STATS = FOREACH_STAT(COUNT_STATS);

#pragma GCC diagnostic pop

#endif
