#include <limits>
#include <cstdlib>
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

bool StraightRoad::inside(Vec2d<float> pos) const {
	float road_rotation = this->getRotation();
	pos -= a;
	pos.rotate(-road_rotation);
	return pos.x >= 0 && pos.x <= length
			&& pos.y >= -(width / 2) && pos.y <= width / 2;
}

Vec2d<float> StraightRoad::randomPointInside() const {
	float r_x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float r_y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	Vec2d<float> rand_pos {r_x, r_y};
	rand_pos.x *= length;
	rand_pos.y -= 0.5f;
	rand_pos.y *= width;
	rand_pos.rotate(rotation);
	rand_pos += a;
	return rand_pos;
}

float StraightRoad::sensor_reading(Vec2d<float> sensor_origin, float angle) const {
	if (!inside(sensor_origin)) {
		return -1.0f;
	}
	const float road_rotation = this->getRotation();
	const float transformed_angle = angle - road_rotation;
	float sin_angle = std::sin(transformed_angle);
	float cos_angle = std::cos(transformed_angle);

	const float eps = 0.0001;
	if (sin_angle == 0) {
		sin_angle += eps;
	}
	if (cos_angle == 0) {
		cos_angle += eps;
	}

	sensor_origin -= a;
	sensor_origin.rotate(-road_rotation);

	float w, h, theta;
	if (sin_angle > 0 && cos_angle > 0) {
		w = length - sensor_origin.x;
		h = width / 2 - sensor_origin.y;
		theta = transformed_angle;
	} else if (sin_angle > 0 && cos_angle < 0) {
		w = sensor_origin.x;
		h = width / 2 - sensor_origin.y;
		theta = M_PI - transformed_angle;
	} else if (sin_angle < 0 && cos_angle > 0) {
		w = length - sensor_origin.x;
		h = sensor_origin.y + width / 2;
		theta = - transformed_angle;
	} else if (sin_angle < 0 && cos_angle < 0) {
		w = sensor_origin.x;
		h = sensor_origin.y + width / 2;
		theta = transformed_angle + M_PI;
	}

	if (h > w * std::tan(theta)) {
		return w / std::cos(theta);
	} else {
		return h / std::sin(theta);
	}
}

float StraightRoad::getLength() const {
	return length;
}

float StraightRoad::getRotation() const {
	return rotation;
}

FilledSquare::FilledSquare(Vec2d<float> location, float side_len, float rotation)
		: location(location), side_len(side_len), rotation(rotation) {}

FilledSquare::~FilledSquare() {}

bool FilledSquare::inside(Vec2d<float> pos) const {
	pos -= location;
	pos.rotate(-rotation);
	return pos.x >= -side_len / 2.0f && pos.y >= -side_len / 2.0f
			&& pos.x <= side_len / 2.0f && pos.y <= side_len / 2.0f;
}

Vec2d<float> FilledSquare::randomPointInside() const {
	float r_x = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float r_y = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	Vec2d<float> rand_pos {r_x, r_y};
	rand_pos.x -= 0.5f;
	rand_pos.y -= 0.5f;
	rand_pos *= side_len;
	rand_pos.rotate(rotation);
	rand_pos += location;
	return rand_pos;
}

float FilledSquare::sensor_reading(Vec2d<float> sensor_origin, float angle) const {
	if (!inside(sensor_origin)) {
		return 0.0f;
	}
	const float road_rotation = rotation;
	const float transformed_angle = angle - road_rotation;
	float sin_angle = std::sin(transformed_angle);
	float cos_angle = std::cos(transformed_angle);

	const float eps = 0.0001;
	if (sin_angle == 0) {
		sin_angle += eps;
	}
	if (cos_angle == 0) {
		cos_angle += eps;
	}

	sensor_origin -= location;
	sensor_origin.rotate(-road_rotation);

	float w, h, theta;
	if (sin_angle > 0 && cos_angle > 0) {
		w = side_len / 2.0f - sensor_origin.x;
		h = side_len / 2.0f - sensor_origin.y;
		theta = transformed_angle;
	} else if (sin_angle > 0 && cos_angle < 0) {
		w = sensor_origin.x + side_len / 2.0f;
		h = side_len / 2.0f - sensor_origin.y;
		theta = M_PI - transformed_angle;
	} else if (sin_angle < 0 && cos_angle > 0) {
		w = side_len / 2.0f - sensor_origin.x;
		h = sensor_origin.y + side_len / 2.0f;
		theta = - transformed_angle;
	} else if (sin_angle < 0 && cos_angle < 0) {
		w = sensor_origin.x + side_len / 2.0f;
		h = sensor_origin.y + side_len / 2.0f;
		theta = transformed_angle + M_PI;
	}

	if (h > w * std::tan(theta)) {
		return w / std::cos(theta);
	} else {
		return h / std::sin(theta);
	}
}

Vec2d<float> FilledSquare::getLocation() const {
	return location;
}

float FilledSquare::getSideLen() const {
	return side_len;
}

float FilledSquare::getRotation() const {
	return rotation;
}

FilledCircularPiece::FilledCircularPiece(Vec2d<float> position, float radius)
	: RoadPiece(), position(position), radius(radius) {}

FilledCircularPiece::~FilledCircularPiece() {}

bool FilledCircularPiece::inside(Vec2d<float> pos) const {
	pos -= this->position;
	return pos.abs() <= radius;
}

Vec2d<float> FilledCircularPiece::randomPointInside() const {
	float r_angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	float r_radius = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
	r_angle *= 2 * M_PI;
	r_radius *= radius;
	float r_x = r_radius * std::cos(r_angle);
	float r_y = r_radius * std::sin(r_angle);
	return Vec2d<float> {r_x, r_y};
}

float FilledCircularPiece::sensor_reading(Vec2d<float> pos, float angle) const {
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
		if (road_piece->inside(car.getPos())) {
			return true;
		}
	}
	return false;
}

Vec2d<float> RoadSystem::randomPointOnRoad() {
	int i = std::rand() % road_pieces.size();		
	Vec2d<float> random_point = road_pieces[i]->randomPointInside();
	return random_point;
}

vector<float> RoadSystem::sensor_readings(
		const CarMechanics &car, const vector<float>& sensor_angles) const
{
	vector<float> readings;
	readings.reserve(sensor_angles.size());

	for (float angle : sensor_angles) {
		float car_adjusted_angle = car.getRotation() + angle;
		float cos_sensor = std::cos(car_adjusted_angle);
		float sin_sensor = std::sin(car_adjusted_angle);

		Vec2d<float> car_pos = car.getPos();
		Vec2d<float> sensor_endpoint = car.getPos();

		float prev_total_reading = -1.0f;
		float total_reading = 0.0f;

		while (total_reading != prev_total_reading) {

			prev_total_reading = total_reading;

			float reading = 0.0f;
			for (RoadPiece* road_piece : road_pieces) {
				float r = road_piece->sensor_reading(sensor_endpoint, car_adjusted_angle);
				reading = std::max(reading, r);
			}

			total_reading += reading;

			float sensor_x = total_reading * cos_sensor + car_pos.x;
			float sensor_y = total_reading * sin_sensor + car_pos.y;
			sensor_endpoint = Vec2d<float> {sensor_x, sensor_y};
		}

		readings.push_back(total_reading);
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
