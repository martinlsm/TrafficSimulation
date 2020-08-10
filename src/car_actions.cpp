#include "car_actions.h"

namespace traffic {

#define LIGHT_TURN (1.0f / 800.0f * 2.0f * M_PI)
#define MEDIUM_TURN (1.0f / 400.0f * 2.0f * M_PI)
#define HARD_TURN (1.0f / 75.0f * 2.0 * M_PI)
void do_action(CarMechanics* car, const car_action action) {
	// handle steering
	if (action & TURN_LEFT_HARD) {
		car->turn(-HARD_TURN);
	} else if (action & TURN_LEFT_MEDIUM) {
		car->turn(-MEDIUM_TURN);
	} else if (action & TURN_LEFT_LIGHT) {
		car->turn(-LIGHT_TURN);
	} else if (action & TURN_RIGHT_HARD) {
		car->turn(HARD_TURN);
	} else if (action & TURN_RIGHT_MEDIUM) {
		car->turn(MEDIUM_TURN);
	} else if (action & TURN_RIGHT_LIGHT) {
		car->turn(HARD_TURN);
	}

	// handle gasing and braking
	if (action & BRAKE_HARD) {
		car->changeSpeed(-1.00f);
	} else if (action & BRAKE_MEDIUM) {
		car->changeSpeed(-0.33f);
	} else if (action & BRAKE_LIGHT) {
		car->changeSpeed(-0.10f);
	} else if (action & GAS_HARD) {
		car->changeSpeed(0.50f);
	} else if (action & GAS_MEDIUM) {
		car->changeSpeed(0.17f);
	} else if (action & GAS_LIGHT) {
		car->changeSpeed(0.05f);
	}
}

} // namespace traffic
