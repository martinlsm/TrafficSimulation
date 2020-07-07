#include "roadway.h"

namespace traffic {

	Road::Road(const Vec2d<float>& a, const Vec2d<float>& b, float width)
			: a(a), b(b), width(width) {}

	StraightRoad::StraightRoad(const Vec2d<float>& a,
			const Vec2d<float>& b, float width)
				: Road(a, b, width),
				rotation(std::atan2(b.y - a.y, b.x - a.x)),
   				length(std::hypot(b.x - a.x, b.y - a.y)) {}

	bool StraightRoad::inside(const CarBody &car) const {
		float road_rotation = this->getRotation();
		Vec2d<float> car_pos = car.getPos();
		car_pos -= a;
		car_pos.rotate(-road_rotation);
		return car_pos.x >= 0 && car_pos.x <= length
				&& car_pos.y >= -(width / 2) && car_pos.y <= width / 2;
	}

	float StraightRoad::getLength() const {
		return length;
	}

	float StraightRoad::getRotation() const {
		return rotation;
	}

	Crossing::Crossing(Vec2d<float> position, float radius)
		: position(position), radius(radius) {}

	bool Crossing::inside(const CarBody &car) const {
		Vec2d<float> car_pos = car.getPos();
		car_pos -= this->position;
		return car_pos.abs() <= radius;
	}

	float Crossing::getRadius() const {
		return radius;
	}

	Vec2d<float> Crossing::getPos() const {
		return position;
	}

	Roadway::~Roadway() {
		for (Road* r : roads) {
			delete r;
		}
	}
	
	void Roadway::addCrossing(const Vec2d<float> location) {
		crossings.push_back(location);
	}

	void Roadway::addRoad(Road* road) {
		roads.push_back(road);
	}

	bool Roadway::inside(const CarBody &car) const {
		for (Road* road : roads) {
			if (road->inside(car)) {
				return true;
			}
		}
		return false;
	}

	float Road::getWidth() const {
		return width;
	}

	Vec2d<float> Road::getCrossingA() const {
		return a;
	}

	Vec2d<float> Road::getCrossingB() const {
		return b;
	}

} // traffic
