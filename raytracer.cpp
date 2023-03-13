#include "raytracer.h"
using namespace std;

void raytracer() {
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	RGBQUAD color;

	//PRINTING STUFF
	/*fprintf(stderr, "WIDTH: %i, HEIGHT: %i\n", width, height);

	fprintf(stderr, "Camera: %f %f %f %f %f %f %f %f %f %f\n", eyeinit[0], eyeinit[1], eyeinit[2],
		centerinit[0], centerinit[1], centerinit[2], upinit[0], upinit[1], upinit[2], fovy);

	fprintf(stderr, "NUM SPHERES:[%i], NUM TRIANGLES:[%i]\n", spheres.size(), triangles.size());*/

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {

			vec3 ray = RayThruPixel(eyeinit - centerinit, upinit, (float)(i + 0.5), (float)(j + 0.5));
			float currentMin = 1000000.0;

			int type = 0; //0 if no intersect, 1 if sphere, 2 if triangle
			for (int k = 0; k < spheres.size(); k++) {
				float t = spheres[k].findIntersection(centerinit, ray);
				//if (t < 0) continue;
				if (t < currentMin && t > 0) {
					currentMin = t;
					type = 1;
				}
			}

			for (int k = 0; k < triangles.size(); k++) {
				float t = triangles[k].findIntersection(centerinit, ray);
				//if (t < 0) continue;
				if (t < currentMin && t > 0) {
					currentMin = t;
					type = 2;
				}
			}

			if (currentMin < 0) {
				//set color for the image to be the ambient color
				color.rgbRed = ambient[0];
				color.rgbGreen = ambient[1];
				color.rgbBlue = ambient[2];
			}
			if (type == 1) { //intersect with a triangle
				//fprintf(stderr, "HITTING A SPHERE\n");
				//set the color to the color of the triangle
				color.rgbRed = 255.0;
				color.rgbGreen = 0.0;
				color.rgbBlue = 0.0;
			} else if (type == 2){
				//fprintf(stderr, "HITTING A TRIANGLE\n");
				color.rgbRed = 0.0;
				color.rgbGreen = 0.0;
				color.rgbBlue = 255.0;
			}
			else if (type == 0) {
				//fprintf(stderr, "HITTING NOTHING\n");
			}

			FreeImage_SetPixelColor(bitmap, i, j, &color);
		}
	}
	input_filename = input_filename.replace(input_filename.find(".test"), input_filename.size(), ".png");
	//fprintf(stderr, input_filename.c_str());
	FreeImage_Save(FIF_PNG, bitmap, input_filename.c_str(), 0);
	FreeImage_DeInitialise();
}

vec3 RayThruPixel(vec3 a, vec3 b, float i, float j) {
	vec3 w = normalize(a);
	vec3 u = normalize(cross(b, w));
	vec3 v = cross(w, u);
	float halfHeight = (float)height / 2.0;
	float halfWidth = (float)width / 2.0;
	float alpha = tan(fovx / 2) * (((float)j - halfWidth) / (float)halfWidth);
	float beta = tan(fovy / 2) * (((float)halfHeight - i) / (float)halfHeight);

	vec3 p1 = (alpha * u) + (beta * v) - w;

	return p1;
}