#pragma once

#include "car.h"
#include <cstdint>

namespace traffic {

typedef std::int64_t car_action;
extern const car_action BRAKE_LIGHT;
extern const car_action BRAKE_MEDIUM;
extern const car_action BRAKE_HARD;
extern const car_action GAS_LIGHT;
extern const car_action GAS_MEDIUM;
extern const car_action GAS_HARD;
extern const car_action TURN_LEFT_LIGHT;
extern const car_action TURN_LEFT_MEDIUM;
extern const car_action TURN_LEFT_HARD;
extern const car_action TURN_RIGHT_LIGHT;
extern const car_action TURN_RIGHT_MEDIUM;
extern const car_action TURN_RIGHT_HARD;

void do_action(CarMechanics* car, const car_action action);

} // namespace traffic
