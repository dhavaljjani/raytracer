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
EXTERN string input_filename;
EXTERN string output_filename;
EXTERN mat4 modelview;
EXTERN int max_verts;

EXTERN float ambient[3];
EXTERN float diffuse[3];
EXTERN float emission[3];
EXTERN float specular[3];
EXTERN float shininess;
EXTERN float width;
EXTERN float height;
EXTERN int max_depth;
EXTERN vec3 sphere_center;
EXTERN vector<vec3> vertices;

EXTERN vector<Light> lights;
EXTERN vec3 attenuation;