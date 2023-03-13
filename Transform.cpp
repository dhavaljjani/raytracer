// Transform.cpp: implementation of the Transform class.

// Note: when you construct a matrix using mat4() or mat3(), it will be COLUMN-MAJOR
// Keep this in mind in readfile.cpp and display.cpp
// See FAQ for more details or if you're having problems.

#include "Transform.h"

// Helper rotation function.  Please implement this.  
mat3 Transform::rotate(const float degrees, const vec3& axis) 
{
    const float radians = glm::radians(degrees);

    mat3 identity(1, 0, 0, 0, 1, 0, 0, 0, 1);
    //mat3 a(axis.x, 0, 0, 0, axis.y, 0, 0, 0, axis.z); //Matrix form of vector a
    //mat3 a_transpose = glm::transpose(a); //A^T
    mat3 a_dualmatrix(0, axis.z, -axis.y, -axis.z, 0, axis.x, axis.y, -axis.x, 0); //Manually defined the dual matrix of A

    //Axis-Angle Formula in matrix form
    mat3 a_atranspose(axis.x * axis.x, axis.x * axis.y, axis.x * axis.z, axis.y * axis.x, axis.y * axis.y, axis.y * axis.z,
        axis.z * axis.x, axis.z * axis.y, axis.z * axis.z);
    mat3 rotation = (cos(radians) * identity) + ((1 - cos(radians)) * a_atranspose) + (sin(radians) * a_dualmatrix);
    //mat3 rotation = (identity * cos(radians)) + ((a * a_transpose) * (1 - cos(radians))) + (a_dualmatrix * (sin(radians)));

  // You will change this return call
    return rotation;
}

void Transform::left(float degrees, vec3& eye, vec3& up) 
{
    printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
    printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", up.x, up.y, up.z, sqrt(pow(up.x, 2) + pow(up.y, 2) + pow(up.z, 2)));
    //Normalize and multiply by 7??
    eye = eye * rotate(-degrees, up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) 
{
    printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", eye.x, eye.y, eye.z, sqrt(pow(eye.x, 2) + pow(eye.y, 2) + pow(eye.z, 2)));
    printf("Coordinates: %.2f, %.2f, %.2f; distance: %.2f\n", up.x, up.y, up.z, sqrt(pow(up.x, 2) + pow(up.y, 2) + pow(up.z, 2)));
    glm::vec3 orthogonal_c = glm::normalize(glm::cross(up, eye));
    eye = eye * rotate(degrees, orthogonal_c);
    up = up * rotate(degrees, orthogonal_c);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) 
{
    //New Coordinate Frame
    glm::vec3 w = glm::normalize(eye);
    glm::vec3 u = glm::normalize(glm::cross(up, w));
    glm::vec3 v = glm::cross(w, u);

    mat4 rotation(u.x, v.x, w.x, 0, u.y, v.y, w.y, 0, u.z, v.z, w.z, 0, 0, 0, 0, 1);
    mat4 translation(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -eye.x, -eye.y, -eye.z, 1);

    // You will change this return call
    return (rotation * translation);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar)
{
    //USE OTHER DEFINITION
    float degrees = fovy / 2;
    float d = (1 / glm::tan(glm::radians(degrees)));
    mat4 ret = mat4((1 / aspect), 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, (-1 / d), 0, 0, 0, 0);
    // YOUR CODE FOR HW2 HERE
    // New, to implement the perspective transform as well.  
    //return (ret * d);
    return mat4(d / aspect, 0, 0, 0, 0, d, 0, 0, 0, 0, (-zFar + zNear) / (zFar - zNear), -1, 0, 0, (-2 * zFar * zNear) / (zFar - zNear), 0);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) 
{
    mat4 ret = mat4(sx, 0, 0, 0, 0, sy, 0, 0, 0, 0, sz, 0, 0, 0, 0, 1);
    return ret;
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) 
{
    mat4 ret = mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, tx, ty, tz, 1);
    return ret;
}

// To normalize the up direction and construct a coordinate frame.  
// As discussed in the lecture.  May be relevant to create a properly 
// orthogonal and normalized up. 
// This function is provided as a helper, in case you want to use it. 
// Using this function (in readfile.cpp or display.cpp) is optional.  

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) 
{
    vec3 x = glm::cross(up, zvec);
    vec3 y = glm::cross(zvec, x);
    vec3 ret = glm::normalize(y);
    return ret;
}


Transform::Transform()
{

}

Transform::~Transform()
{

}
