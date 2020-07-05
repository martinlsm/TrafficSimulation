#include "roadway.h"

namespace traffic {

	Road::Road(const Vec2d<float>& a, const Vec2d<float>& b, float width)
			: a(a), b(b), width(width) {}

	bool StraightRoad::inside(const CarBody &car) const {
		// TODO: implement
		return true;
	}

	float StraightRoad::getLength() const {
		float x = a.x - b.x;
		float y = a.y - b.y;	
		return std::sqrt(x*x + y*y);
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
