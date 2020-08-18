#include <limits>
#include "road_system.h"

namespace traffic {

StraightRoad::StraightRoad(const Vec2d<float> a,
		const Vec2d<float> b, float width)
			: RoadPiece(),
			a(a),
			b(b),
			width(width),
			rotation(std::atan2(b.y - a.y, b.x - a.x)),
			length(std::hypot(b.x - a.x, b.y - a.y)) {}

StraightRoad::~StraightRoad() {}

bool StraightRoad::inside(const CarMechanics &car) const {
	float road_rotation = this->getRotation();
	Vec2d<float> car_pos = car.getPos();
	car_pos -= a;
	car_pos.rotate(-road_rotation);
	return car_pos.x >= 0 && car_pos.x <= length
			&& car_pos.y >= -(width / 2) && car_pos.y <= width / 2;
}

float StraightRoad::sensor_reading(const CarMechanics &car, float angle) const {
	if (!inside(car)) {
		return std::numeric_limits<float>::max();
	}
	float road_rotation = this->getRotation();
	float actual_angle = car.getRotation() + angle - road_rotation; // or + road_rotation?
	float sin_angle = std::sin(actual_angle);

	Vec2d<float> car_pos = car.getPos();
	car_pos -= a;
	car_pos.rotate(-road_rotation);

	float f_eps = 0.001;
	if (sin_angle > f_eps) {
		float dist_to_upper_border = width / 2 - car_pos.y;
		return dist_to_upper_border / sin_angle;
	} else if (sin_angle < -f_eps) {
		float dist_to_lower_border = width / 2 + car_pos.y;
		return -dist_to_lower_border / sin_angle;
	} else {
		return std::numeric_limits<float>::max();
	}
}

float StraightRoad::getLength() const {
	return length;
}

float StraightRoad::getRotation() const {
	return rotation;
}

FilledCircularPiece::FilledCircularPiece(Vec2d<float> position, float radius)
	: RoadPiece(), position(position), radius(radius) {}

FilledCircularPiece::~FilledCircularPiece() {}

bool FilledCircularPiece::inside(const CarMechanics &car) const {
	Vec2d<float> car_pos = car.getPos();
	car_pos -= this->position;
	return car_pos.abs() <= radius;
}

float FilledCircularPiece::sensor_reading(const CarMechanics &car, float angle) const {
	throw "TODO: Function not implemented";
}

float FilledCircularPiece::getRadius() const {
	return radius;
}

Vec2d<float> FilledCircularPiece::getPos() const {
	return position;
}

RoadSystem::RoadSystem(vector<RoadPiece*> road_pieces) {
	this->road_pieces = road_pieces;
}

RoadSystem::~RoadSystem() {
	for (RoadPiece* r : road_pieces) {
		delete r;
	}
}

bool RoadSystem::inside(const CarMechanics &car) const {
	for (RoadPiece* road_piece : road_pieces) {
		if (road_piece->inside(car)) {
			return true;
		}
	}
	return false;
}

vector<float> RoadSystem::sensor_readings(
		const CarMechanics &car, const vector<float>& sensor_angles) const
{
	vector<float> readings;
	readings.reserve(sensor_angles.size());

	for (float sensor : sensor_angles) {
		float reading = std::numeric_limits<float>::max();
		for (RoadPiece* road_piece : road_pieces) {
			float temp_reading = road_piece->sensor_reading(car, sensor);
			reading = std::min(reading, temp_reading);
		}
		readings.push_back(reading);
	}
	return readings;
}

float StraightRoad::getWidth() const {
	return width;
}

Vec2d<float> StraightRoad::getEndpointA() const {
	return a;
}

Vec2d<float> StraightRoad::getEndpointB() const {
	return b;
}

} // namespace traffic
