#include "kappa.h"

#include <iostream> 

void update_state(int size, double *state) {
	for (int i = 0; i < size; i++) {
		incr(&state[i]);
	}
}
