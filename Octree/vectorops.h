#pragma once
#include <math.h>
#include <immintrin.h>

struct point {
	union {
		struct { 
			float x, y, z; 
		};
	};

	float length() const {
		return sqrtf(x * x + y * y + z * z);
	}
	void norm() { 
		x = x/length();
		y = y/length();
		z = z/length();
	};

	point operator*(float a) const {
		return point{ x*a, y*a, z*a}; 
	}
	point operator+(const point& v) const { 
		return point{ x + v.x, y + v.y, z + v.z }; 
	}
	point operator-(const point& v) const { 
		return point{ x - v.x, y - v.y, z - v.z }; 
	}

	void operator+=(const point& v) { 
		x += v.x; y += v.y; z += v.z; 
	}
	void operator-=(const point& v) { 
		x -= v.x; y -= v.y; z -= v.z; 
	}

	bool operator==(const point& v) const { 
		return fabsf(x - v.x) < 0.000001 && 
			   fabsf(y - v.y) < 0.000001 && 
			   fabsf(z - v.z) < 0.000001; 
	}
};

inline float dot(const point& v1, const point& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline point cross(const point& v1, const point& v2) {
	point p = { v1.y*v2.z - v1.z*v2.y, v1.z*v2.x - v1.x*v2.z, v1.x*v2.y - v1.y*v2.x };
	return p;
}
