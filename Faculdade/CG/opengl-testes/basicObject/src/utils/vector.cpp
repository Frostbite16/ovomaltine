#include "vector.h"

Vector3f_t::Vector3f_t(float x, float y, float z){
	vec[0] = x;
	vec[1] = y;
	vec[2] = z;
}

void Vector3f_t::setX(float x){
	vec[0] = x;
}


