#pragma once

#include <cmath>
#include <iostream>

#include "polar_vec2d.h"

namespace geometry {

template <class T>
struct Vec2d {
	T x,y;
	Vec2d(const T x, const T y);
	const float abs();
	void normalize();
	void rotate(const float rad);
	void operator+=(const Vec2d<T> &rhs);
	void operator+=(const PolarVec2d<T> &rhs);
	void operator-=(const Vec2d<T> &rhs);

	template<class U>
	friend Vec2d<U> operator+(const Vec2d<U> &v1, const Vec2d<U> &v2);

	template<class U>
	friend Vec2d<U> operator-(const Vec2d<U> &v1, const Vec2d<U> &v2);

	template<class U>
	friend Vec2d<U> operator*(const U lhs, const Vec2d<U> &rhs);

	template<class U>
	friend std::ostream& operator<<(std::ostream &os, const Vec2d<U> &v);

	template<class U>
	friend bool operator==(const Vec2d<U> &rhs, const Vec2d<U> &lhs);

	template<class U>
	friend bool operator!=(const Vec2d<U> &rhs, const Vec2d<U> &lhs);
};

template <class T>
Vec2d<T>::Vec2d(const T x, const T y) : x(x), y(y) {}

template <class T>
const float Vec2d<T>::abs() {
	return std::sqrt(x * x + y * y);
}

template <class T>
void Vec2d<T>::normalize() {
	float len = this->abs();
	x /= len;
	y /= len;
}

template <class T>
void Vec2d<T>::rotate(const float rad) {
	float sin = std::sin(rad);	
	float cos = std::cos(rad);
	float new_x = cos * x - sin * y;
	float new_y = sin * x + cos * y;
	x = new_x;
	y = new_y;
}

template <class T>
void Vec2d<T>::operator+=(const Vec2d<T> &rhs) {
	x += rhs.x;
	y += rhs.y;
}

template <class T>
void Vec2d<T>::operator+=(const PolarVec2d<T> &rhs) {
	T rhs_x = std::cos(rhs.v) * rhs.r;
	T rhs_y = std::sin(rhs.v) * rhs.r;
	x += rhs_x;
	y += rhs_y;
}

template <class T>
void Vec2d<T>::operator-=(const Vec2d<T> &rhs) {
	x -= rhs.x;
	y -= rhs.y;
}

template <class T>
Vec2d<T> operator+(const Vec2d<T> &v1, const Vec2d<T> &v2) {
	return Vec2d<T>(v1.x + v2.x, v1.y + v2.y);
}

template <class T>
Vec2d<T> operator-(const Vec2d<T> &v1, const Vec2d<T> &v2) {
	return Vec2d<T>(v1.x - v2.x, v1.y - v2.y);
}

template <class T>
Vec2d<T> operator*(const T lhs, const Vec2d<T> &rhs) {
	return Vec2d<T>(lhs * rhs.x, lhs * rhs.y);
}

template <class T>
std::ostream& operator<<(std::ostream &os, const Vec2d<T> &v) {
	return os << "(" << v.x << ", " << v.y << ")";
}

template <class T>
bool operator==(const Vec2d<T> &rhs, const Vec2d<T> &lhs) {
	return rhs.x == lhs.x && lhs.y == rhs.y;
}

template <class T>
bool operator!=(const Vec2d<T> &rhs, const Vec2d<T> &lhs) {
	return !(rhs == lhs);
}

} // geometry
