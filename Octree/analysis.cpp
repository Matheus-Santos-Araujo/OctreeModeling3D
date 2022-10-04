#include <iostream>
#include <math.h>

_inline void areaEsf(float r) {
	float result = 4 * 3.14159265 * (r*r);
	std::cout << "Area esfera = " << result << std::endl;
}

_inline void volEsf(float r) {
	float result = 4 * (3.14159265 * (r*r*r)) / 3;
	std::cout << "Volume esfera = " << result << std::endl;
}

_inline void areaCilin(float r, float a) {
	float result = 2 * 3.14159265 * r * (a + r);
	std::cout << "Area do cilindro = " << result << std::endl;
}

_inline void volCilin(float r, float a) {
	float result = 3.14159265 * (r*r) * a;
	std::cout << "Volume do cilindro = " << result << std::endl;
}

_inline void areaCube(float a) {
	float result = 2 * (a * a + a * a + a * a);
	std::cout << "Area do cube = " << result << std::endl;
}

_inline void volCube(float a) {
	float result = a * a * a;
	std::cout << "Volume do cube = " << result << std::endl;
}

_inline void areaCone(float r, float a) {
	float result = 3.14159265 * r * (r + sqrtf(r*r) + (a*a));
	std::cout << "Area do cone = " << result << std::endl;
}

_inline void volCone(float r, float a) {
	float result = (3.14159265 * (r*r) * a) / 3;
	std::cout << "Volume do cone = " << result << std::endl;
}

