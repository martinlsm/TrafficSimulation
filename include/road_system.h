#pragma once

#include <vector>

#include "vec2d.h"
#include "car.h"

using std::vector;
using geometry::Vec2d;

namespace traffic {

class RoadPiece {
public:
	virtual ~RoadPiece() {};
	virtual bool inside(Vec2d<float> pos) const = 0;
	virtual Vec2d<float> randomPointInside() const = 0;

	/* If the sensor is placed within the road piece, this function returns the
	 * distance between the sensor's placement and sensor's ray cross the
	 * border of this road piece.
	 *
	 * If the car is NOT within the road piece, this function will return -1
	 * 
	 * sensor_origin: Where the sensor is located
	 * angle: Which direction the sensor's ray is pointing
	 *
	 * returns: The distance between the sensor and the border of this road
	 * piece, given the angle the sensor's ray is aiming
	 */
	virtual float sensor_reading(Vec2d<float> sensor_origin, float angle) const = 0;
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
	~StraightRoad() override;
	bool inside(Vec2d<float> pos) const override;
	Vec2d<float> randomPointInside() const override;
	float sensor_reading(Vec2d<float> sensor_origin, float angle) const override;
	float getWidth() const;
	Vec2d<float> getEndpointA() const;
	Vec2d<float> getEndpointB() const;
	float getRotation() const;
	float getLength() const;
};

class FilledSquare : public RoadPiece {
private:
	const Vec2d<float> location;
	const float side_len;
	const float rotation;
public:
	FilledSquare(Vec2d<float> location, float side_len, float rotation);
	~FilledSquare() override;
	bool inside(Vec2d<float> pos) const override;
	Vec2d<float> randomPointInside() const override;
	float sensor_reading(Vec2d<float> sensor_origin, float angle) const override;
	Vec2d<float> getLocation() const;
	float getSideLen() const;
	float getRotation() const;
};

class FilledCircularPiece : public RoadPiece {
private:
	Vec2d<float> position;
	const float radius;
public:
	FilledCircularPiece(Vec2d<float> position, float radius);
	~FilledCircularPiece() override;
	bool inside(Vec2d<float> pos) const override;
	Vec2d<float> randomPointInside() const override;
	float sensor_reading(Vec2d<float> sensor_origin, float angle) const override;
	float getRadius() const;
	Vec2d<float> getPos() const;
};

class RoadSystem {
public:
	vector<RoadPiece*> road_pieces;

	RoadSystem(vector<RoadPiece*> road_pieces);
	~RoadSystem();

	/* Returns true if the entire car is on the road, otherwise false */
	bool inside(const CarMechanics &car) const;

	Vec2d<float> randomPointOnRoad();

	vector<float> sensor_readings(
			const CarMechanics &car, const vector<float>& sensor_angles) const;
};

} // namespace traffic
