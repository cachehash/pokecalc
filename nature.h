//	hinder		boost
//	S SD	SA D A|S 	SD SA D A
#define FOREACH_NAT(D) \
D(HARDY, 0x21) \
D(LONELY, 0x41) \
D(ADAMANT, 0x81) \
D(NAUGHTY, 0x101) \
D(BRAVE, 0x201) \
D(BOLD, 0x22) \
D(DOCILE, 0x42) \
D(IMPISH, 0x82) \
D(LAX, 0x102) \
D(RELAXED, 0x202) \
D(MODEST, 0x24) \
D(MILD, 0x44) \
D(BASHFULL, 0x84) \
D(RASH, 0x104) \
D(QUIET, 0x204) \
D(CALM, 0x28) \
D(GENTLE, 0x48) \
D(CAREFUL, 0x88) \
D(QUIRKY, 0x108) \
D(SASSY, 0x208) \
D(TIMID, 0x30) \
D(HASTY, 0x50) \
D(JOLLY, 0x90) \
D(NAIVE, 0x110) \
D(SERIOUS, 0x210)

#define GENERATE_NAT_ENUM(TOK, VAL) TOK,
#define GENERATE_NAT_MAP(TOK, VAL) VAL,
#define GENERATE_NAT_STRINGS(TOK, VAL) #TOK,
#define GENERATE_NATURE_COUNT(TOK, VAL) +1

enum NAT_ENUM {
	FOREACH_NAT(GENERATE_NAT_ENUM)
};

static const int NAT_MAP[] = {
	FOREACH_NAT(GENERATE_NAT_MAP)
};
static const char* NAT_STRINGS[] = {
	FOREACH_NAT(GENERATE_NAT_STRINGS)
};

static const int NUM_NATS = FOREACH_NAT(GENERATE_NATURE_COUNT);

int natureBoost(int nat, int statID);
int natureHinder(int nat, int statID);
float natureFactor(int nat, int statID);