#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include "Transform.h"
#include "readfile.h"
using namespace std;
#define MAINPROGRAM 
#include "variables.h" 
#include "raytracer.h"

int main(int argc, char* argv[]) {

    if (argc < 2) {
        cerr << "Usage: transforms scenefile [grader input (optional)]\n";
        exit(-1);
    }

    readfile(argv[1]);
    raytracer();

    return 0;
}
