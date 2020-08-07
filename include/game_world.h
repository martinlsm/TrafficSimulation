#pragma once

#include <vector>

#include "road_system.h"
#include "constants.h"

namespace traffic {

/*
 * Cars spawn and end at Destinations
 */
struct Destination {
	const Vec2d<float> position;
	const float direction;
	Destination(Vec2d<float> position, float direction);
	CarBody placeCar();
	bool inside(const Vec2d<float> p, const float margin) const;
};

struct TrafficEnvironment {
	RoadSystem road_system;
	vector<Destination> destinations;
	vector<CarBody> active_cars;
};

TrafficEnvironment load_environment(unsigned int id);

} // namespace traffic
