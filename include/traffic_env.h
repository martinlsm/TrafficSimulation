#pragma once

#include <vector>
#include <unordered_map>

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
	CarMechanics placeCar();
	bool inside(const Vec2d<float> p, const float margin) const;
};

typedef std::int8_t car_state;
const car_state NOT_FOUND    = 0; // is not in the traffic environment
const car_state INACTIVE     = 1; // just spawned and has not received any state
const car_state ON_ROAD      = 2;
const car_state OFF_ROAD     = 3;
const car_state REACHED_GOAL = 4;

struct Car {
	CarMechanics body;
	Destination* goal;
	car_state state = INACTIVE;
};

class TrafficEnvironment {
private:
	vector<Destination> destinations;
	std::unordered_map<unsigned long, Car> active_cars;
	unsigned long car_counter = 0;
public:
	RoadSystem* road_system;

	TrafficEnvironment(vector<Destination> dests, RoadSystem* road_system);
	TrafficEnvironment(const TrafficEnvironment&) = delete;

	car_state getCarState(const unsigned long car_id) const;
	CarMechanics* getCarMechanics(unsigned long car_id);
	size_t destination_count() const;
	size_t car_count() const;

	// Spawns a car and returns its assigned id
	unsigned long spawn_car(size_t start_idx, size_t goal_idx);
	void update(const float dt);
	void clearFinishedCars();
};

TrafficEnvironment* load_environment(unsigned int id);

} // namespace traffic
