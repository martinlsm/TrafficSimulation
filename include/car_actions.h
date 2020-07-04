#pragma once

#include "car.h"
#include <cstdint>

namespace CarAction {

typedef std::int64_t car_action;
const car_action BRAKE_LIGHT      = 1L << 0;
const car_action BRAKE_MEDIUM     = 1L << 1;
const car_action BRAKE_HARD       = 1L << 2;
const car_action GAS_LIGHT        = 1L << 3;
const car_action GAS_MEDIUM       = 1L << 4;
const car_action GAS_HARD         = 1L << 5;
const car_action TURN_LEFT_LIGHT  = 1L << 6;
const car_action TURN_LEFT_HARD   = 1L << 7;
const car_action TURN_RIGHT_LIGHT = 1L << 8;
const car_action TURN_RIGHT_HARD  = 1L << 9;

class CarActionController {
private:
	CarBody *car_body;
public:
	CarActionController(CarBody *car_body);
	void doAction(const car_action action);
};

} // CarAction