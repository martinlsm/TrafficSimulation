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

bool StraightRoad::inside(Vec2d<float> pos) const {
	float road_rotation = this->getRotation();
	pos -= a;
	pos.rotate(-road_rotation);
	return pos.x >= 0 && pos.x <= length
			&& pos.y >= -(width / 2) && pos.y <= width / 2;
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

FilledCircularPiece::FilledCircularPiece(Vec2d<float> position, float radius)
	: RoadPiece(), position(position), radius(radius) {}

FilledCircularPiece::~FilledCircularPiece() {}

bool FilledCircularPiece::inside(Vec2d<float> pos) const {
	pos -= this->position;
	return pos.abs() <= radius;
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

vector<float> RoadSystem::sensor_readings(
		const CarMechanics &car, const vector<float>& sensor_angles) const
{
	vector<float> readings;
	readings.reserve(sensor_angles.size());

	for (float angle : sensor_angles) {
		float car_adjusted_angle = car.getRotation() + angle;
		bool found_reading = false;
		float cos_sensor = std::cos(car_adjusted_angle);
		float sin_sensor = std::sin(car_adjusted_angle);

		Vec2d<float> car_pos = car.getPos();
		Vec2d<float> sensor_endpoint = car.getPos();
		Vec2d<float> prev_sensor_endpoint {-1.0f, -1.0f};

		float total_reading = 0.0f;

		while ((sensor_endpoint - prev_sensor_endpoint).abs() > 1e-5) {

			prev_sensor_endpoint = sensor_endpoint;

			float reading = -1.0f;
			for (RoadPiece* road_piece : road_pieces) {
				float r = road_piece->sensor_reading(sensor_endpoint, car_adjusted_angle);
				if (r >= 0) {
					reading = std::max(reading, r);
					found_reading = true;
				}
			}

			if (reading > 0) {
				total_reading += reading;
			}

			float sensor_x = total_reading * cos_sensor + car_pos.x;
			float sensor_y = total_reading * sin_sensor + car_pos.y;
			sensor_endpoint = Vec2d<float> {sensor_x, sensor_y};
		}

		if (!found_reading) {
			total_reading = 0.0f;
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
