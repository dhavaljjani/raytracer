#include <iostream>
#include <fstream>
#include <sstream>
#include "Transform.h" 
#include "readfile.h"
#include "variables.h"
using namespace std;

Sphere::Sphere(vec3 center, float r) {
    this->center = center;
    this->r = r;
}

float Sphere::findIntersection(vec3 p0, vec3 p1) {

    float A = glm::dot(p1, p1);
    //Is this meant to be center or centerinit?
    float B = 2 * dot(p1, p0 - center);
    float C = dot(p0 - center, p0 - center) - (r * r);

    float discriminant = (B * B) - (4.0 * A * C);
    //fprintf(stderr, "A:[%f], B:[%f], C:[%f]\n", A, B, C);
    //fprintf(stderr, "DOS: [%f]\n", discriminant);

    if (discriminant < 0) return -1;//case where there is no intersection

    float intOne = (-B - discriminant) / (2.0 * A);
    float intTwo = (-B + discriminant) / (2.0 * A);
    //fprintf(stderr, "1:[%f], 2:[%f]\n", intOne, intTwo);

    if (intOne > 0 && intTwo > 0) {
        return intOne;
    }
    else if (intOne < 0 && intTwo > 0) {
        return intTwo;
    }
    else if (intOne == intTwo) {
        return intOne;
    }
    else {
        return -1;
    }

    //these two lines return negative values of t, only in the case where both are negative
    //otherwise it returns the greatest positive integer
    if (intOne < 0) return intTwo;
    if (intTwo < 0) return intOne;


    //in the case where both values are positive, return the smallest t value
    if (intOne < intTwo) return intOne;
    return intTwo;
}


Light::Light(vec3 posn, vec3 color, bool isPoint) {
    this->light_posn = posn;
    this->color = color;
    this->isPoint = isPoint;
}

Triangle::Triangle(vec3 A, vec3 B, vec3 C) {
    this->A = A;
    this->B = B;
    this->C = C;
}

float Triangle::findIntersection(vec3 p0, vec3 p1) {
    vec3 normal = glm::cross(C - A, B - A);
    normal = normalize(normal);

    if (glm::dot(p1, normal) == 0) return -1; //ray is parallel to plane, no intersection
    float t = (glm::dot(A, normal) - glm::dot(p0, normal)) / (glm::dot(p1, normal)); //find t value for plane intersection

    vec3 P = p0 + t * p1;

    //formulas taken from discussion slide as directed
    vec3 APNorm = glm::cross(normal, C - B) / (glm::dot(glm::cross(normal, C - B), A - C));
    float APw = glm::dot(-APNorm, C);

    float a = glm::dot(APNorm, P) + APw;
    if (a < 0 || a > 1) return -1;

    vec3 BPNorm = glm::cross(normal, A - C) / (glm::dot(glm::cross(normal, A - C), B - A));
    float BPw = glm::dot(-BPNorm, A);

    float b = glm::dot(BPNorm, P) + BPw;

    if (b > 1 || b < 0) return -1;


    vec3 CPNorm = glm::cross(normal, B - A) / (glm::dot(glm::cross(normal, B - A), C - B));
    float CPw = glm::dot(-CPNorm, B);

    float c = glm::dot(CPNorm, P) + CPw;

    if (c < 0 || c > 1) return -1;
    return t;
}

// You may not need to use the following two functions, but it is provided
// here for convenience

// The function below applies the appropriate transform to a 4-vector
void matransform(stack<mat4> &transfstack, int values[])
{
    mat4 transform = transfstack.top(); 
    vec4 valvec = vec4(values[0],values[1],values[2],values[3]); 
    vec4 newval = transform * valvec; 
    for (int i = 0; i < 4; i++) values[i] = newval[i]; 
}

void rightmultiply(const mat4 & M, stack<mat4> &transfstack) 
{
    mat4 &T = transfstack.top(); 
    T = T * M; 
}

// Function to read the input data values
// Use is optional, but should be very helpful in parsing.  
bool readvals(stringstream &s, const int numvals, int values[])
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i]; 
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n"; 
            return false;
        }
    }
    return true; 
}

bool readvals(stringstream& s, const int numvals, float values[])
{
    for (int i = 0; i < numvals; i++) {
        s >> values[i];
        if (s.fail()) {
            cout << "Failed reading value " << i << " will skip\n";
            return false;
        }
    }
    return true;
}

void readfile(const char* filename) 
{
    string str, cmd; 
    ifstream in;
    in.open(filename); 
    if (in.is_open()) {

        // I need to implement a matrix stack to store transforms.  
        // This is done using standard STL Templates 
        transfstack.push(mat4(1.0));
        input_filename = filename;
        attenuation = vec3(1, 0, 0);
        getline (in, str); 
        while (in) {
            if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {
                stringstream s(str);
                s >> cmd; 
                int i;
                int size_values[2];
                float values[10];
                bool validinput; // Validity of input 
                //fprintf(stderr, "COMMAND: %s\n", cmd);
                // Process the light, add it to database.
                // Lighting Command
                if (cmd == "point") {
                    validinput = readvals(s, 6, values); // Position/color for lts.
                    vec3 light_posn;
                    vec3 light_color;
                    if (validinput) {
                        for (int i = 0; i < 3; i++) {
                            light_posn[i] = values[i];
                        }
                        for (i = 3; i < 6; i++) {
                            light_color[i - 3] = values[i];
                        }
                        Light l = Light(light_posn, light_color, true);
                    }
                }
                else if (cmd == "directional") {
                    validinput = readvals(s, 6, values); // Position/color for lts.
                    vec3 light_posn;
                    vec3 light_color;
                    if (validinput) {
                        for (int i = 0; i < 3; i++) {
                            light_posn[i] = values[i];
                        }
                        for (i = 3; i < 6; i++) {
                            light_color[i - 3] = values[i];
                        }
                        Light l = Light(light_posn, light_color, false);
                    }
                }
                else if (cmd == "attenuation") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (int i = 0; i < 3; i++) {
                            attenuation[i] = values[i];
                        }
                    }
                }

                // Material Commands 
                // Ambient, diffuse, specular, shininess properties for each object.
                // Filling this in is pretty straightforward, so I've left it in 
                // the skeleton, also as a hint of how to do the more complex ones.
                // Note that no transforms/stacks are applied to the colors. 

                else if (cmd == "ambient") {
                    validinput = readvals(s, 3, values); // colors 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            ambient[i] = values[i]; 
                        }
                    }
                } else if (cmd == "diffuse") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            diffuse[i] = values[i]; 
                        }
                    }
                } else if (cmd == "specular") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            specular[i] = values[i]; 
                        }
                    }
                } else if (cmd == "emission") {
                    validinput = readvals(s, 3, values); 
                    if (validinput) {
                        for (i = 0; i < 3; i++) {
                            emission[i] = values[i]; 
                        }
                    }
                } else if (cmd == "shininess") {
                    validinput = readvals(s, 1, values); 
                    if (validinput) {
                        shininess = values[0]; 
                    }
                } else if (cmd == "size") {
                    validinput = readvals(s,2, size_values);
                    if (validinput) { 
                        width = (int)size_values[0];
                        height = (int)size_values[1];
                    } 
                } else if (cmd == "camera") {
                    validinput = readvals(s,10,values); // 10 values eye cen up fov
                    if (validinput) {
                        eyeinit = vec3(values[0], values[1], values[2]);
                        centerinit = vec3(values[3], values[4], values[5]);
                        upinit = vec3(values[6], values[7], values[8]);
                        fovy = values[9];
                        upinit = Transform::upvector(upinit, eyeinit - centerinit);
                        fovx = 2.0 * atan(tan((float)(fovy / 2.0)) * ((width) / (height)));
                    }
                }

                // I've left the code for loading objects in the skeleton, so 
                // you can get a sense of how this works.  
                // Also look at demo.txt to get a sense of why things are done this way.
                else if (cmd == "sphere" || cmd == "tri" || cmd == "vertex"
                    || cmd == "maxverts" || cmd == "maxvertsnorms" || cmd == "vertexnormal"
                    || cmd == "trinormal") {
                    if (cmd == "vertex") {
                        validinput = readvals(s, 3, values);
                        for (i = 0; i < 3; i++) {
                            vertex[i] = values[i];
                        }
                        vertices.push_back(vertex);
                    }
                    else if (cmd == "sphere") {
                        validinput = readvals(s, 4, values);
                        for (i = 0; i < 3; i++) {
                            sphere_center[i] = values[i];
                        }
                        Sphere s = Sphere(sphere_center, values[3]);
                        spheres.push_back(s);
                    }
                    else if (cmd == "tri") {
                        validinput = readvals(s, 3, values);
                        Triangle t = Triangle(vertices[values[0]], vertices[values[1]], vertices[values[2]]);
                        triangles.push_back(t);
                    }
                    else if (cmd == "vertexnormal" || cmd == "maxverts" || cmd == "maxvertsnorms" || cmd == "vertexnormal") {
                        //Nothing
                    }
                }

                else if (cmd == "translate") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
                        rightmultiply(Transform::translate(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "scale") {
                    validinput = readvals(s,3,values); 
                    if (validinput) {
                        rightmultiply(Transform::scale(values[0], values[1], values[2]), transfstack);
                    }
                }
                else if (cmd == "rotate") {
                    validinput = readvals(s,4,values); 
                    if (validinput) {
                        vec3 axis = glm::normalize(vec3(values[0], values[1], values[2]));
                        rightmultiply(glm::mat4(Transform::rotate(values[3], axis)), transfstack);
                    }
                }

                // I include the basic push/pop code for matrix stacks
                else if (cmd == "pushTransform") {
                    transfstack.push(transfstack.top()); 
                } else if (cmd == "popTransform") {
                    if (transfstack.size() <= 1) {
                        cerr << "Stack has no elements.  Cannot Pop\n"; 
                    } else {
                        transfstack.pop(); 
                    }
                }
                else if (cmd == "output") {
                    validinput = readvals(s, 1, values);
                    if (validinput) {
                        output_filename = values[0];
                    }
                }
                else if (cmd == "maxdepth") {
                    max_depth = 5;
                }

                else {
                    cerr << "Unknown Command: " << cmd << " Skipping \n"; 
                }
            }
            getline (in, str); 
        }

        // Set up initial position for eye, up and amount
        // As well as booleans 

        /*eye = eyeinit;
        up = upinit; 
        amount = 5;
        sx = sy = 1.0;  // keyboard controlled scales in x and y 
        tx = ty = 0.0;  // keyboard controllled translation in x and y  */

    } else {
        cerr << "Unable to Open Input Data File " << filename << "\n"; 
        throw 2; 
    }
}