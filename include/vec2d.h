#pragma once

#include <cmath>

namespace geometry {

template <class T>
struct Vec2d {
	T x,y;
	Vec2d(const T x, const T y);
	const float abs();
	void operator+=(const Vec2d<T> &rhs);

	template<class U>
	friend Vec2d<U> operator+(const Vec2d<U> &v1, const Vec2d<U> &v2);
};

template <class T>
Vec2d<T>::Vec2d(const T x, const T y) : x(x), y(y) {}

template <class T>
const float Vec2d<T>::abs() {
	return std::sqrt(x * x + y * y);
}

template <class T>
void Vec2d<T>::operator+=(const Vec2d<T> &rhs) {
	x += rhs.x;
	y += rhs.y;
}

template <class T>
Vec2d<T> operator+(const Vec2d<T> &v1, const Vec2d<T> &v2) {
	return Vec2d<T>(v1.x + v2.x, v1.y + v2.y);
}

} // geometry
