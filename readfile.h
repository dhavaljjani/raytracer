// Readfile definitions
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include <vector>
using namespace std;

class Sphere {
    public:
        vec3 center;
        float r;
        Sphere(vec3 center, float r);

    float findIntersection(vec3 p0, vec3 p1);
};

class Triangle {
    public:
        vec3 A;
        vec3 B;
        vec3 C;
        Triangle(vec3 A, vec3 B, vec3 C);

        float findIntersection(vec3 p0, vec3 p1);
};

class Light {
    vec3 light_posn;
    vec3 color;
    bool isPoint;

    public:
        Light(vec3 posn, vec3 color, bool isPoint);
};

void matransform (stack<mat4> &transfstack, int values[]);
void rightmultiply (const mat4 & M, stack<mat4> &transfstack) ;
bool readvals (stringstream &s, const int numvals, int values[]);
void readfile (const char * filename) ;
