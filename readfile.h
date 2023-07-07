// Readfile definitions
#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>
#include <vector>
using namespace std;

class Sphere {
public:
    vec3 sphere_center;
    float r;
    float color_ambient[3] = { 0, 0, 0 };
    float object_emission[3] = { 0, 0, 0 };
    float object_specular[3] = {0, 0, 0};
    float object_diffuse[3] = {0, 0, 0};
    mat4 transform;
    Sphere(vec3 center_input, float r);

    float findIntersection(vec3 p0, vec3 p1);
    void setColorAmbient(float a, float b, float c);
    void setObjectEmission(float a, float b, float c);
    void setObjectSpecular(float a, float b, float c);
    void setObjectDiffuse(float a, float b, float c);
};

class Triangle {
public:
    vec3 A;
    vec3 B;
    vec3 C;
    float color_ambient[3] = { 0, 0, 0 };
    float object_emission[3] = { 0, 0, 0 };
    float object_specular[3] = {0, 0, 0};
    float object_diffuse[3] = {0, 0, 0};
    mat4 transform;
    Triangle(vec3 A, vec3 B, vec3 C);

    float findIntersection(vec3 p0, vec3 p1);
    void setColorAmbient(float a, float b, float c);
    void setObjectEmission(float a, float b, float c);
    void setObjectSpecular(float a, float b, float c);
    void setObjectDiffuse(float a, float b, float c);
};

class Light {

public:
    vec3 light_posn;
    vec3 color;
    bool isPoint;
    Light(vec3 posn, vec3 color, bool isPoint);
};

class Intersection {
    public:
        float t;
        int type;
        vec4 p0; vec4 p1;
        vec3 normal;
        bool hit;
        bool sphere_hit; bool triangle_hit;
        float currentMin;
        float current_ambient[3]{};
        float object_emission[3]{};
        float object_specular[3]{};
        float object_diffuse[3]{};
};

void matransform(stack<mat4>& transfstack, int values[]);
void rightmultiply(const mat4& M, stack<mat4>& transfstack);
bool readvals(stringstream& s, const int numvals, float values[], string cmd);
void readfile(const char* filename);