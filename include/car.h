#pragma once

#include "rectangle.h"

using geometry::Rectangle;

class Car {
private:
	Rectangle rect;
public:
	Car(const double center_x, const double center_y);
};
