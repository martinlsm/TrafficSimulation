#include "car_actions.h"

namespace CarAction {

CarActionController::CarActionController(CarMechanics *car_body) {
	this->car_body = car_body;
}

#define LIGHT_TURN (1.0f / 800.0f * 2.0f * M_PI)
#define MEDIUM_TURN (1.0f / 400.0f * 2.0f * M_PI)
#define HARD_TURN (1.0f / 75.0f * 2.0 * M_PI)
void CarActionController::doAction(const car_action action) {
	// handle steering
	if (action & TURN_LEFT_HARD) {
		car_body->turn(-HARD_TURN);
	} else if (action & TURN_LEFT_MEDIUM) {
		car_body->turn(-MEDIUM_TURN);
	} else if (action & TURN_LEFT_LIGHT) {
		car_body->turn(-LIGHT_TURN);
	} else if (action & TURN_RIGHT_HARD) {
		car_body->turn(HARD_TURN);
	} else if (action & TURN_RIGHT_MEDIUM) {
		car_body->turn(MEDIUM_TURN);
	} else if (action & TURN_RIGHT_LIGHT) {
		car_body->turn(HARD_TURN);
	}

	// handle gasing and braking
	if (action & BRAKE_HARD) {
		car_body->changeSpeed(-1.00f);
	} else if (action & BRAKE_MEDIUM) {
		car_body->changeSpeed(-0.33f);
	} else if (action & BRAKE_LIGHT) {
		car_body->changeSpeed(-0.10f);
	} else if (action & GAS_HARD) {
		car_body->changeSpeed(0.50f);
	} else if (action & GAS_MEDIUM) {
		car_body->changeSpeed(0.17f);
	} else if (action & GAS_LIGHT) {
		car_body->changeSpeed(0.05f);
	}
}

} // CarAction
