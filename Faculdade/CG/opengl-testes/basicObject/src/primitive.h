#include "GL/glut.h"
#include "utils/vector.h"
#include "GL/freeglut.h"
#include <GL/gl.h>

class primitive_t{
	Vector3f_t pos_from_camera;
	float c_g, c_r, c_b;
	public:
		primitive_t(Vector3f_t pos_from_camera, float c_g, float c_r, float c_b);
		void set_transformations();
		void draw_object();
};

inline void primitive_t::set_transformations(){
	glTranslatef(pos_from_camera.getX(), pos_from_camera.getY(), pos_from_camera.getZ());
}
