// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"
#include <glm/gtc/reciprocal.inl>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    mat3 ret;
    // YOUR CODE FOR HW2 HERE
    // Please implement this.  Likely the same as in HW 1.  
    const float radian = glm::radians(degrees);

    // skew-symmetric(dual) matrix (for cross product)
    glm::mat3 A = { 0, (-1.0f * axis.z), axis.y,
                    axis.z, 0, (-1.0f * axis.x),
                    (-1.0f * axis.y), axis.x, 0 };

    // compute rotation matrix around axis
    ret = glm::cos(radian) * glm::mat3(1.f) + (1.f - glm::cos(radian)) * glm::outerProduct(axis, axis) + glm::sin(radian) * A;

    return ret;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1. 
    eye = eye * rotate(degrees, up);
    up = up * rotate(degrees, up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    // YOUR CODE FOR HW2 HERE 
    // Likely the same as in HW 1.  
    vec3 w = glm::normalize(glm::cross(eye, up));

    eye = eye * rotate(degrees, w);
    up = up * rotate(degrees, w);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // Likely the same as in HW 1.  
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
    // change order, because column major order matrix
    ret[0][0] = u.x; ret[0][1] = v.x; ret[0][2] = w.x;
    ret[1][0] = u.y; ret[1][1] = v.y; ret[1][2] = w.y;
    ret[2][0] = u.z; ret[2][1] = v.z; ret[2][2] = w.z;
    ret[3][0] = rt.x;
    ret[3][1] = rt.y;
    ret[3][2] = rt.z;

    return ret;
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    mat4 ret;
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    assert(aspect > 0);

    if (zNear <= 0 || zFar <= 0)
        return mat4(1.f);
   
    const float theta = glm::radians(fovy) / 2.f;

    // float d = 1 / glm::tan(theta);
    float d = glm::cot(theta);
    float A = -(zFar + zNear) / (zFar - zNear);
    float B = -(2 * zFar * zNear) / (zFar - zNear);

    mat4 P(1.f);
    P[0][0] = d / aspect;
    P[1][1] = d;
    P[2][2] = A;
    P[2][3] = B;
    P[3][2] = -1.f;
    P[3][3] = 0.f;
    
    ret = glm::transpose(P);

    return ret;
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret(1.f);
    // YOUR CODE FOR HW2 HERE
    // Implement scaling 
    ret[0][0] = sx;
    ret[1][1] = sy;
    ret[2][2] = sz;

    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret(1.f);
    // YOUR CODE FOR HW2 HERE
    // Implement translation 
    ret[0][3] = tx;
    ret[1][3] = ty;
    ret[2][3] = tz;
    // transpose for column-major(opengl)
    return glm::transpose(ret);
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up,zvec); 
    vec3 y = glm::cross(zvec,x); 
    vec3 ret = glm::normalize(y); 
    return ret; 
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
