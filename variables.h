#pragma once
#include "Transform.h"
#include "readfile.h"

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 centerinit;
EXTERN vec3 vertex;
EXTERN float fovy;
EXTERN float fovx;
EXTERN stack<mat4> transfstack;
EXTERN vector<Sphere> spheres;
EXTERN vector<Triangle> triangles;
EXTERN string output_filename;

EXTERN int ambient[3];
EXTERN int diffuse[3];
EXTERN int emission[3];
EXTERN int specular[3];
EXTERN int shininess;
EXTERN int width;
EXTERN int height;
EXTERN int max_depth;
EXTERN vec3 sphere_center;
EXTERN vector<vec3> vertices;

EXTERN vector<vec3> lights;
EXTERN vec3 attenuation;