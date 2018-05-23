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
