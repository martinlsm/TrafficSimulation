// void update_state(int size, double *state);

void update_state(int size, double *state) {
	for (int i = 0; i < size; i++) {
		state[i] += 1.0;
	}
}
