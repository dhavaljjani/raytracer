#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stack>
#include <FreeImage.h>
#include "variables.h"
#include "readfile.h"

void raytracer();
vec3 RayThruPixel(vec3 a, vec3 b, float i, float j);