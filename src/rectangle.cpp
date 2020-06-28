#include "rectangle.h"

namespace geometry {
	Rectangle::Rectangle(const float center_x, const float center_y,
			const float width, const float height)
				: center(center_x, center_y), width(width), height(height) {}
} // geometry
