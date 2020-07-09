#pragma once

#include <vector>

#include "roadway.h"
#include "constants.h"

namespace traffic {

struct Destination {
	const Vec2d<float> position;
	const float direction;
	Destination(Vec2d<float> position, float direction);
	CarBody placeCar();
	bool inside(const Vec2d<float> p, const float margin) const;
};

/*
 * Returns all road pieces for a game world and possible starting-
 * and end points for cars.
 */
std::pair<vector<RoadPiece*>, vector<Destination>> load_world(unsigned int id);

} // traffic
