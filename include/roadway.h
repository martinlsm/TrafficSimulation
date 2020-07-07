#pragma once

#include <vector>

#include "vec2d.h"
#include "car.h"

using std::vector;
using geometry::Vec2d;

namespace traffic {

class Road {
protected:
	/* crossings a and b */
	const Vec2d<float>& a;
	const Vec2d<float>& b;
	float width;
public:
	Road(const Vec2d<float>& a, const Vec2d<float>& b, float width);
	virtual bool inside(const CarBody &car) const = 0;
	float getWidth() const;
	Vec2d<float> getCrossingA() const;
	Vec2d<float> getCrossingB() const;
};

class StraightRoad : public Road {
private:
	/* cached as fields for faster computation */
	const float rotation; // [-pi/2, pi/2]
	const float length;
public:
	StraightRoad(const Vec2d<float>& a, const Vec2d<float>& b, float width);
	virtual bool inside(const CarBody &car) const;
	float getRotation() const;
	float getLength() const;
};

class Crossing {
private:
	Vec2d<float> position;
	const float radius;
public:
	Crossing(Vec2d<float> position, float radius);
	bool inside(const CarBody &car) const;
	float getRadius() const;
	Vec2d<float> getPos() const;
};

class Roadway {
private:
	/* coordinates of each crossing */
	vector<Vec2d<float>> crossings;

	/* roads (edges) between each crossing,
	 * represented as indices in the crossings vector */
	vector<Road*> roads;
public:
	~Roadway();
	void addCrossing(const Vec2d<float> location);
	void addRoad(Road* road);

	/* Returns true if the entire car is on the road, otherwise false */
	bool inside(const CarBody &car) const;
};

} // traffic
