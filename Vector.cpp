#include "Vector.h"
#include <math.h>

Vector3Matrix Add(const Vector3Matrix& v1, const Vector3Matrix& v2) {
	Vector3Matrix add = {v1.x + v2.x, v1.y + v2.y, v1.z + v2.z};
	return add;
}

Vector3Matrix Subtract(const Vector3Matrix& v1, const Vector3Matrix& v2) {
	Vector3Matrix subtract = {v1.x + -v2.x, v1.y - v2.y, v1.z - v2.z};
	return subtract;
}

Vector3Matrix Multiply(float scalar, const Vector3Matrix& v) {
	Vector3Matrix multply = {scalar * v.x, scalar * v.y, scalar * v.z};
	return multply;
}

float Dot(const Vector3Matrix& v1, const Vector3Matrix& v2) { return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z); }

float Length(const Vector3Matrix& v) { return sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z)); }

Vector3Matrix Normalize(const Vector3Matrix& v) {
	float length = sqrtf((v.x * v.x) + (v.y * v.y) + (v.z * v.z));

	Vector3Matrix normalize = v;
	if (length != 0) {
		normalize.x = v.x / length;
		normalize.y = v.y / length;
		normalize.z = v.z / length;
	}
	return normalize;
}
