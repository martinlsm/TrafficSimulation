#include "roadway.h"

namespace traffic {

	Road::Road(const Vec2d<float>& a, const Vec2d<float>& b, float width)
			: a(a), b(b), width(width) {}

	StraightRoad::StraightRoad(const Vec2d<float>& a,
			const Vec2d<float>& b, float width)
				: Road(a, b, width),
				rotation(std::atan((b.y - a.y) / (b.x - a.x))),
   				length(std::hypot(b.x - a.x, b.y - a.y)) {}

	bool StraightRoad::inside(const CarBody &car) const {
		float road_rotation = this->getRotation();
		Vec2d<float> car_pos = car.getPos();
		car_pos.rotate(-road_rotation);
		return (car_pos.x >= std::min(a.x, b.x))
				&& (car_pos.x <= std::max(a.x, b.x))
				&& (car_pos.y >= std::min(a.y, b.y))
				&& (car_pos.y <= std::max(a.y, b.y));
	}

	float StraightRoad::getLength() const {
		return length;
	}

	float StraightRoad::getRotation() const {
		return rotation;
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
