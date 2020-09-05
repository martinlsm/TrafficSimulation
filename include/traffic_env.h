#pragma once

#include <vector>
#include <unordered_map>

#include "road_system.h"
#include "constants.h"
#include "car_actions.h"

namespace traffic {

/*
 * Cars spawn and end on Destinations
 */
struct Destination {
	const Vec2d<float> position;
	const float direction;
	Destination(Vec2d<float> position, float direction);
	CarMechanics placeCar();
	bool inside(const Vec2d<float> p, const float margin) const;
};

typedef std::int8_t car_state;
extern const car_state NOT_FOUND;
extern const car_state INACTIVE;
extern const car_state ON_ROAD;
extern const car_state ON_ROAD_STANDING_STILL;
extern const car_state OFF_ROAD;
extern const car_state REACHED_GOAL;

struct Car {
	CarMechanics body;
	vector<float> sensor_angles;
	Destination* goal;
	car_state state;

	Car(CarMechanics body, Destination* goal);
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
	size_t destinationCount() const;
	Vec2d<float> getCarDestination(unsigned long car_id) const;
	vector<float> getCarSensorReadings(unsigned long car_id) const;
	size_t carCount() const;
	std::vector<unsigned long> get_car_ids();
	void removeCar(unsigned long car_id);

	// Spawns a car and returns its assigned id
	unsigned long spawnCar(size_t start_idx, size_t goal_idx);
	unsigned long spawnCarAtRandomPos(size_t goal_idx);
	void doAction(unsigned long car_id, car_action action);
	void update(const float dt);
	void clearFinishedCars();
};

TrafficEnvironment* load_environment(unsigned int id);

} // namespace traffic
