#include "car_actions.h"

namespace CarAction {

CarActionController::CarActionController(CarBody *car_body) {
	this->car_body = car_body;
}

#define LIGHT_TURN (1.0f / 50.0f * 2.0f * M_PI)
#define HARD_TURN (1.0f / 100.0f * 2.0f * M_PI)
void CarActionController::doAction(const car_action action) {
	// handle steering
	if (action & TURN_LEFT_HARD) {
		car_body->turn(HARD_TURN);
	} else if (action & TURN_LEFT_LIGHT) {
		car_body->turn(LIGHT_TURN);
	} else if (action & TURN_RIGHT_HARD) {
		car_body->turn(-HARD_TURN);
	} else if (action & TURN_RIGHT_LIGHT) {
		car_body->turn(-HARD_TURN);
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
