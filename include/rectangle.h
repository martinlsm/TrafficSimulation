#pragma once

#include "vec2d.h"

namespace geometry {

struct Rectangle {
	Rectangle(const float x, const float y,
			const float width, const float height);
	Vec2d<float> center;
	float width, height;
	float orientation;   // [0, 2*pi]
};

} // geometry
