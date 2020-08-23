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


const car_state NOT_FOUND              = 0; // is not in the traffic environment
const car_state INACTIVE               = 1; // just spawned and has not received any state
const car_state ON_ROAD                = 2;
const car_state ON_ROAD_STANDING_STILL = 3;
const car_state OFF_ROAD               = 4;
const car_state REACHED_GOAL           = 5;

Car::Car(CarMechanics body, Destination* goal) : body(body), goal(goal), state(INACTIVE) {
	for (int i = 0; i < 10; i++) {
		float angle = i * 2.0f * M_PI / 10.0f;
		sensor_angles.push_back(angle);
	}
}

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

size_t TrafficEnvironment::destinationCount() const {
	return destinations.size();
}

size_t TrafficEnvironment::carCount() const {
	return active_cars.size();
}

Vec2d<float> TrafficEnvironment::getCarDestination(unsigned long car_id) const {
	auto it = active_cars.find(car_id);
	const Car* car = &it->second;
	return car->goal->position;
}


vector<float> TrafficEnvironment::getCarSensorReadings(unsigned long car_id) const {
	auto it = active_cars.find(car_id);
	const CarMechanics& car = it->second.body;
	const vector<float>& sensor_angles = it->second.sensor_angles;
	return road_system->sensor_readings(car, sensor_angles);
}

unsigned long TrafficEnvironment::spawnCar(size_t start_idx, size_t goal_idx) {
	car_counter++;
	Destination* start = &destinations[start_idx];
	Destination* goal = &destinations[goal_idx];
	active_cars.insert(std::pair<unsigned int, Car>(car_counter, Car{start->placeCar(), goal}));
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
		} else if (std::abs(car.body.getSpeed()) < 0.05f) {
			car.state = ON_ROAD_STANDING_STILL;
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

void TrafficEnvironment::removeCar(unsigned long car_id) {
	active_cars.erase(car_id);
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

	dests.emplace_back(A, M_PI / 2.0f);
	dests.emplace_back(B, M_PI / 2.0f);
	dests.emplace_back(C, 0.0f);
	dests.emplace_back(F, M_PI);
	dests.emplace_back(G, 0.0f);
	dests.emplace_back(J, M_PI);
	dests.emplace_back(K, 3.0f * M_PI / 2.0f);
	dests.emplace_back(L, 3.0f * M_PI / 2.0f);
}

/*
 * A
 * |
 * B--C
 *    |
 *    D
 */
void load_world_2(vector<RoadPiece*> &v, vector<Destination> &dests) {
	Vec2d<float> A {WORLD_WIDTH / 3.0f, 0.0f};
	Vec2d<float> B {WORLD_WIDTH / 3.0f, WORLD_HEIGHT / 2.0f};
	Vec2d<float> C {2.0f * WORLD_WIDTH / 3.0f, WORLD_HEIGHT / 2.0f};
	Vec2d<float> D {2.0f * WORLD_WIDTH / 3.0f, WORLD_HEIGHT};
	float road_width = 100.0f;

	v.push_back(new StraightRoad(A, B, road_width));
	v.push_back(new StraightRoad(B, C, road_width));
	v.push_back(new StraightRoad(C, D, road_width));
	v.push_back(new FilledSquare(B, road_width, 0.0f));
	v.push_back(new FilledSquare(C, road_width, 0.0f));

	dests.emplace_back(A, M_PI / 2.0f);
	dests.emplace_back(D, 3.0f * M_PI / 2.0f);
}

void load_world_3(vector<RoadPiece*> &v, vector<Destination> &dests) {
	Vec2d<float> center {WORLD_WIDTH / 2.0f, WORLD_HEIGHT / 2.0f};
	float side_len = std::min(WORLD_HEIGHT, WORLD_WIDTH) / 2.0f;
	Vec2d<float> A {center.x - side_len / 2.5f, center.y - side_len / 2.5f};
	Vec2d<float> B {center.x + side_len / 2.5f, center.y + side_len / 2.5f};

	v.push_back(new FilledSquare(center, side_len, 0.0f));

	dests.emplace_back(A, M_PI / 2.0f);
	dests.emplace_back(B, 3.0f * M_PI / 2.0f);
}

TrafficEnvironment* load_environment(unsigned int id) {
	vector<RoadPiece*> v;
	vector<Destination> dests;
	switch (id) {
		case 1:
			load_world_1(v, dests);
			break;
		case 2:
			load_world_2(v, dests);
			break;
		case 3:
			load_world_3(v, dests);
			break;
	}

	if (v.empty()) {
		throw std::invalid_argument("invalid world id");
	}

	RoadSystem* road_system = new RoadSystem(v);
	TrafficEnvironment* env = new TrafficEnvironment{dests, road_system};
	return env;
}

std::vector<unsigned long> TrafficEnvironment::get_car_ids() {
	std::vector<unsigned long> car_ids;
	for (auto it = active_cars.begin(); it != active_cars.end(); ++it) {
		car_ids.push_back(it->first);
	}
	return car_ids;
}

} // namespace traffic
