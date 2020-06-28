#pragma once

#include "vec2d.h"

namespace geometry {

struct Rectangle {
	Rectangle(const double x, const double y);
	Vec2d<double> center;
	double width, height;
	double orientation;   // [0, 2*pi]
};

} // geometry
