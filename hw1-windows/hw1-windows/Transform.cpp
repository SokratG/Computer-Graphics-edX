// Transform.cpp: implementation of the Transform class.

#include "Transform.h"
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
  // YOUR CODE FOR HW1 HERE
	const float radian = glm::radians(degrees);

	// skew-symmetric(dual) matrix (for cross product)
	glm::mat3 A = { 0, (-1.0f * axis.z), axis.y,
					axis.z, 0, (-1.0f * axis.x),
					(-1.0f * axis.y), axis.x, 0 };

	// compute rotation matrix around axis
	glm::mat3 Rot_ax = glm::cos(radian) * glm::mat3(1.f) + (1.f - glm::cos(radian)) * glm::outerProduct(axis, axis) + glm::sin(radian) * A;

	// You will change this return call
	// return glm::rotate(radian, axis);
	return Rot_ax;
}

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE

	eye = eye * rotate(degrees, up);
	up = up * rotate(degrees, up);
	
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
  // YOUR CODE FOR HW1 HERE 
	vec3 w = glm::normalize(glm::cross(eye, up));

	eye = eye * rotate(degrees, w);
	up = up * rotate(degrees, w);

	// printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
  // YOUR CODE FOR HW1 HERE
	// https://www.scratchapixel.com/lessons/mathematics-physics-for-computer-graphics/lookat-function - for help
	// constuct coordinate frame - {u, v, w} camera
	vec3 w = glm::normalize(eye);
	
	vec3 u = glm::normalize(glm::cross(up, w));

	vec3 v = glm::cross(w, u);
	
	// make rotate-translate vector
	mat3 R;
	R[0][0] = u.x; R[0][1] = u.y; R[0][2] = u.z;
	R[1][0] = v.x; R[1][1] = v.y; R[1][2] = v.z;
	R[2][0] = w.x; R[2][1] = w.y; R[2][2] = w.z;
	vec3 rt = -eye * R;


	// make result view matrix(look at)
	mat4 view;
	// change order, because column major order matrix
	view[0][0] = u.x; view[0][1] = v.x; view[0][2] = w.x;
	view[1][0] = u.y; view[1][1] = v.y; view[1][2] = w.y;
	view[2][0] = u.z; view[2][1] = v.z; view[2][2] = w.z;
	view[3][0] = rt.x;
	view[3][1] = rt.y;
	view[3][2] = rt.z;

	// You will change this return call
	return view;
}

Transform::Transform()
{

}

Transform::~Transform()
{

}