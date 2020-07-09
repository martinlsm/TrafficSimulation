#pragma once

#include <vector>

#include "vec2d.h"
#include "car.h"

using std::vector;
using geometry::Vec2d;

namespace traffic {

class RoadPiece {
public:
	virtual bool inside(const CarBody &car) const = 0;
};

class StraightRoad : public RoadPiece {
private:
	/* endpoints a and b */
	const Vec2d<float> a;
	const Vec2d<float> b;
	const float width;

	/* cached as fields for faster computation */
	const float rotation;
	const float length;
public:
	StraightRoad(const Vec2d<float> a, const Vec2d<float> b, float width);
	virtual bool inside(const CarBody &car) const;
	float getWidth() const;
	Vec2d<float> getEndpointA() const;
	Vec2d<float> getEndpointB() const;
	float getRotation() const;
	float getLength() const;
};

class FilledCircularPiece : public RoadPiece {
private:
	Vec2d<float> position;
	const float radius;
public:
	FilledCircularPiece(Vec2d<float> position, float radius);
	bool inside(const CarBody &car) const;
	float getRadius() const;
	Vec2d<float> getPos() const;
};

class Roadway {
private:
	vector<RoadPiece*> road_pieces;
public:
	Roadway(vector<RoadPiece*> road_pieces);
	~Roadway();

	/* Returns true if the entire car is on the road, otherwise false */
	bool inside(const CarBody &car) const;
};

} // traffic
