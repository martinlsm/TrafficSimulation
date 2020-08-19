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
	float road_rotation = this->getRotation();
	float transformed_angle = angle - road_rotation;
	float sin_angle = std::sin(transformed_angle);

	sensor_origin -= a;
	sensor_origin.rotate(-road_rotation);

	float f_eps = 0.001;
	if (sin_angle > f_eps) {
		float dist_to_upper_border = width / 2 - sensor_origin.y;
		return dist_to_upper_border / sin_angle;
	} else if (sin_angle < -f_eps) {
		float dist_to_lower_border = width / 2 + sensor_origin.y;
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

	for (float sensor : sensor_angles) {
		bool found_reading = false;
		float cos_sensor = std::cos(sensor);
		float sin_sensor = std::sin(sensor);

		Vec2d<float> car_pos = car.getPos();
		Vec2d<float> sensor_endpoint = car.getPos();
		Vec2d<float> prev_sensor_endpoint {-1.0f, -1.0f};

		float total_reading = 0.0f;

		while ((sensor_endpoint - prev_sensor_endpoint).abs() < 1e-3) { // TODO: make this cheaper

			prev_sensor_endpoint = sensor_endpoint;

			float reading = -1.0f;
			for (RoadPiece* road_piece : road_pieces) {
				float r = road_piece->sensor_reading(sensor_endpoint, sensor);
				if (r >= 0) {
					reading = std::max(reading, r);
					found_reading = true;
					total_reading += reading;
					break;
				}
			}

			float sensor_x = (sensor_endpoint.x - car_pos.x) * cos_sensor + car_pos.x;
			float sensor_y = (sensor_endpoint.y - car_pos.y) * sin_sensor + car_pos.y;
			sensor_endpoint = Vec2d<float> {sensor_x, sensor_y};
		}

		if (!found_reading) {
			total_reading = std::numeric_limits<float>::max();
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
