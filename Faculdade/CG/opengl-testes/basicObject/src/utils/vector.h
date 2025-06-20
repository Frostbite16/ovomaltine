#ifndef _VECTOR3_H
#define _VECTOR3_H

class Vector3f_t{
	float vec[3];
	public:
		Vector3f_t(float x, float y, float z);

		void setX(float x);
		void setY(float y);
		void setZ(float z);

		float getX();
		float getY();
		float getZ();

		Vector3f_t operator-();
		void operator=(Vector3f_t other);
		Vector3f_t operator*(Vector3f_t other);
		Vector3f_t operator/(Vector3f_t other);
};

inline void Vector3f_t::setX(float num){
	vec[0] = num;
}

inline void Vector3f_t::setY(float num){
	vec[1] = num;
}

inline void Vector3f_t::setZ(float num){
	vec[2] = num;
}

inline float Vector3f_t::getX(){
	return vec[0];
}

inline float Vector3f_t::getY(){
	return vec[1];
}

inline float Vector3f_t::getZ(){
	return vec[2];
}

inline void Vector3f_t::operator=(Vector3f_t other){
	vec[0] = other.vec[0];
    vec[1] = other.vec[1];
    vec[2] = other.vec[2];
}

inline Vector3f_t Vector3f_t::operator*(Vector3f_t other){
	vec[0] *= other.vec[0];
    vec[1] *= other.vec[1];
    vec[2] *= other.vec[2];
	return *this;
}

inline Vector3f_t Vector3f_t::operator/(Vector3f_t other){
	vec[0] /= other.vec[0];
    vec[1] /= other.vec[1];
    vec[2] /= other.vec[2];
	return *this;
}


inline Vector3f_t Vector3f_t::operator-(){
	return Vector3f_t(-this->vec[0], -this->vec[1], -this->vec[2]);
}


#endif
