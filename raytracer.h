#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#include <FreeImage.h>
#include "variables.h"
#include "readfile.h"

void raytracer();
bool isInShadow(vec3 origin, vec3 lightPos);
vec3 RayThruPixel(vec3 a, vec3 b, float i, float j);
vec3 getColor(int type, int index, vec3 position, int depth);