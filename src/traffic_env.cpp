#include <stdexcept>

#include "traffic_env.h"
#include "road_system.h"

namespace traffic {

Destination::Destination(Vec2d<float> position, float direction)
		: position(position), direction(direction) {}

CarMechanics Destination::placeCar() {
	return CarMechanics{position.x, position.y, direction};
}

bool Destination::inside(const Vec2d<float> p, const float margin) const {
	Vec2d<float> diff = p;
	diff -= this->position;
	return diff.abs() <= margin;
}

TrafficEnvironment::TrafficEnvironment(vector<Destination> dests, RoadSystem* road_system)
		: destinations(dests), road_system(road_system) {}


const car_state NOT_FOUND    = 0; // is not in the traffic environment
const car_state INACTIVE     = 1; // just spawned and has not received any state
const car_state ON_ROAD      = 2;
const car_state OFF_ROAD     = 3;
const car_state REACHED_GOAL = 4;

car_state TrafficEnvironment::getCarState(const unsigned long car_id) const {
	auto it = active_cars.find(car_id);
	if (it == active_cars.end()) {
		return NOT_FOUND;
	}
	const Car& car = it->second;
	return car.state;
}

CarMechanics* TrafficEnvironment::getCarMechanics(unsigned long car_id) {
	auto it = active_cars.find(car_id);
	CarMechanics& body = it->second.body;
	return &body;
}

size_t TrafficEnvironment::destination_count() const {
	return destinations.size();
}

size_t TrafficEnvironment::car_count() const {
	return active_cars.size();
}

unsigned long TrafficEnvironment::spawnCar(size_t start_idx, size_t goal_idx) {
	car_counter++;
	Destination* start = &destinations[start_idx];
	Destination* goal = &destinations[goal_idx];
	active_cars.insert(std::pair<unsigned int, Car>(car_counter, Car{start->placeCar(), goal, INACTIVE}));
	return car_counter;
}

void TrafficEnvironment::doAction(unsigned long car_id, car_action action) {
	auto it = active_cars.find(car_id);
	CarMechanics* car = &it->second.body;
	do_action(car, action);
}

void TrafficEnvironment::update(const float dt) {
	for (auto it = active_cars.begin(); it != active_cars.end(); ++it) {
		Car &car = it->second;
		car.body.update(dt);

		// update car's state
		if (!road_system->inside(car.body)) {
			car.state = OFF_ROAD;
		} else if ((car.goal)->inside(car.body.getPos(), GOAL_MARGIN)) {
			car.state = REACHED_GOAL;
		} else {
			car.state = ON_ROAD;
		}
	}
}

void TrafficEnvironment::clearFinishedCars() {
	auto it = active_cars.begin();
	while (it != active_cars.end()) {
		Car &car = it->second;
		if (car.state == OFF_ROAD || car.state == REACHED_GOAL) {
			it = active_cars.erase(it);
		} else {
			++it;
		}
	}
}

/*
 *    A  B
 *    |  |
 * C--D--E--F
 *    |  |
 * G--H--I--J
 *    |  |
 *    K  L
 */
void load_world_1(vector<RoadPiece*> &v, vector<Destination> &dests) {
	Vec2d<float> A {WORLD_WIDTH / 4.0f, 0.0f};
	Vec2d<float> B {3.0f * WORLD_WIDTH / 4.0f, 0.0f};
	Vec2d<float> C {0.0f, WORLD_HEIGHT / 4.0f};
	Vec2d<float> D {WORLD_WIDTH / 4.0f, WORLD_HEIGHT / 4.0f};
	Vec2d<float> E {3.0f * WORLD_WIDTH / 4.0f,  WORLD_HEIGHT / 4.0f};
	Vec2d<float> F {WORLD_WIDTH, WORLD_HEIGHT / 4.0f};
	Vec2d<float> G {0.0f, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> H {WORLD_WIDTH / 4.0f, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> I {3.0f * WORLD_WIDTH / 4.0f, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> J {WORLD_WIDTH, 3.0f * WORLD_HEIGHT / 4.0f};
	Vec2d<float> K {WORLD_WIDTH / 4.0f, WORLD_HEIGHT};
	Vec2d<float> L {3.0f * WORLD_WIDTH / 4.0f, WORLD_HEIGHT};
	float road_width = 75.0f;

	v.push_back(new StraightRoad(A, D, road_width));
	v.push_back(new StraightRoad(B, E, road_width));
	v.push_back(new StraightRoad(C, D, road_width));
	v.push_back(new StraightRoad(D, E, road_width));
	v.push_back(new StraightRoad(E, F, road_width));
	v.push_back(new StraightRoad(D, H, road_width));
	v.push_back(new StraightRoad(E, I, road_width));
	v.push_back(new StraightRoad(G, H, road_width));
	v.push_back(new StraightRoad(H, I, road_width));
	v.push_back(new StraightRoad(I, J, road_width));
	v.push_back(new StraightRoad(H, K, road_width));
	v.push_back(new StraightRoad(I, L, road_width));

	dests.push_back({A, M_PI / 2.0f});
	dests.push_back({B, M_PI / 2.0f});
	dests.push_back({C, 0.0f});
	dests.push_back({F, M_PI});
	dests.push_back({G, 0.0f});
	dests.push_back({J, M_PI});
	dests.push_back({K, 3.0f * M_PI / 2.0f});
	dests.push_back({L, 3.0f * M_PI / 2.0f});
}

TrafficEnvironment* load_environment(unsigned int id) {
	vector<RoadPiece*> v;
	vector<Destination> dests;
	switch (id) {
		case 1:
			load_world_1(v, dests);
			break;
	}

	if (v.empty()) {
		throw std::invalid_argument("invalid world id");
	}

	RoadSystem* road_system = new RoadSystem(v);
	TrafficEnvironment* env = new TrafficEnvironment{dests, road_system};
	return env;
}

} // namespace traffic
