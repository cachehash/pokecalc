#define FOREACH_STAT(D) \
D(Hp) \
D(Atk) \
D(Def) \
D(SpA) \
D(SpD) \
D(Spe) \

#define GENERATE_ENUM(TOK) TOK,
#define GENERATE_STRING(TOK) #TOK,
	
enum STAT_ENUM {
	FOREACH_STAT(GENERATE_ENUM)
};

static const char *STAT_STRINGS[] = {
	FOREACH_STAT(GENERATE_STRING)
};

