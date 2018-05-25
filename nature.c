#include "nature.h"
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
int getNatureID(char* str) {
	for (int n = 0; n < NUM_NATS; n++) {
		const char* nat = NAT_STRINGS[n];
		int match = 1;
		for (int i = 0; str[i] && nat[i]; i++) {
			if (str[i] == '\n') {
				break;
			}
			if ((nat[i] | ' ') != (str[i] | ' ')) {
				match = 0;
				break;
			}
		}
		if (match) {
			return n;
		}
	}
	return -1;
}
