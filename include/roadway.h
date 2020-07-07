#pragma once

#include <vector>

#include "vec2d.h"
#include "car.h"

using std::vector;
using geometry::Vec2d;

namespace traffic {

class RoadPiece {
protected:
	/* endpoints a and b */
	const Vec2d<float> a;
	const Vec2d<float> b;
	const float width;
public:
	RoadPiece(const Vec2d<float> a, const Vec2d<float> b, float width);
	virtual bool inside(const CarBody &car) const = 0;
	float getWidth() const;
	Vec2d<float> getEndpointA() const;
	Vec2d<float> getEndpointB() const;
};

class StraightRoad : public RoadPiece {
private:
	/* cached as fields for faster computation */
	const float rotation; // [-pi/2, pi/2]
	const float length;
public:
	StraightRoad(const Vec2d<float> a, const Vec2d<float> b, float width);
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

	vector<RoadPiece*> roads;
public:
	~Roadway();
	void addCrossing(const Vec2d<float> location);
	void addRoadPiece(RoadPiece* road_piece);

	/* Returns true if the entire car is on the road, otherwise false */
	bool inside(const CarBody &car) const;
};

} // traffic
