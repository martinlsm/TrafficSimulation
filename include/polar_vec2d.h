#pragma once

#include <cmath>
#include <iostream>

namespace geometry {

template <class T>
struct PolarVec2d {
	T r;
	float v;
	PolarVec2d(const T r, const float v);
	const float abs();
	void normalize();
	void rotate(const float rad);

	template<class U>
	friend PolarVec2d<U> operator*(const U lhs, const PolarVec2d<U> &rhs);

	template<class U>
	friend std::ostream& operator<<(std::ostream &os, const PolarVec2d<U> &v);
};

template <class T>
PolarVec2d<T>::PolarVec2d(const T r, const float v) : r(r), v(v) {}

template <class T>
const float PolarVec2d<T>::abs() {
	return static_cast<float>(r);
}

template <class T>
void PolarVec2d<T>::normalize() {
	r = T(1);
}

template <class T>
void PolarVec2d<T>::rotate(const float rad) {
	v += rad;
}

template <class T>
PolarVec2d<T> operator*(const T lhs, const PolarVec2d<T> &rhs) {
	return PolarVec2d<T>(lhs * rhs.r, rhs.v);
}

template <class T>
std::ostream& operator<<(std::ostream &os, const PolarVec2d<T> &v) {
	return os << "polar(" << v.r << ", " << v.v << ")";
}

} // geometry
