#pragma once

#include <vector>

#include "roadway.h"
#include "constants.h"

namespace traffic {

/*
 * Returns all road pieces for a game world and possible starting-
 * and end points for cars.
 */
std::pair<vector<RoadPiece*>, vector<Vec2d<float>>> load_world(unsigned int id);

} // traffic
