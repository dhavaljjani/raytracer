#include "raytracer.h"
using namespace std;

void raytracer() {
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	RGBQUAD color;

	//PRINTING STUFF
	/*fprintf(stderr, "Width: %f, Height: %f\n", width, height);
	fprintf(stderr, "NUM SPHERES:[%i], NUM TRIANGLES:[%i]\n", spheres.size(), triangles.size());
	fprintf(stderr, "Fovy: %f\n", fovy);
	fprintf(stderr, "Fovx: %f\n", fovx);
	for (int k = 0; k < max_verts; k++) {
		fprintf(stderr, "Vertex %i: [%f, %f, %f]\n", k, vertices[k][0], vertices[k][1], vertices[k][2]);
	}
	for (int k = 0; k < spheres.size(); k++) {
		fprintf(stderr, "Sphere: [%f, %f, %f] with radius %f\n", spheres[k].center[0], spheres[k].center[1],
			spheres[k].center[2], spheres[k].r);
	}
	for (int k = 0; k < triangles.size(); k++) {
		fprintf(stderr, "Triangle: [%f, %f, %f] [%f, %f, %f] [%f, %f, %f]\n", triangles[k].A[0], triangles[k].A[1], triangles[k].A[2],
			triangles[k].B[0], triangles[k].B[1], triangles[k].B[2], triangles[k].C[0], triangles[k].C[1], triangles[k].C[2]);
	}*/

	for (float i = 0; i < width; i++) {
		for (float j = 0; j < height; j++) {

			vec3 ray = RayThruPixel(eyeinit - centerinit, upinit, ((float)i + (float)0.5), ((float)j + (float)0.5));
			ray = normalize(ray);

			float currentMin = 1000000.0;

			int type = 0; //0 if no intersect, 1 if sphere, 2 if triangle
			float current_ambient[3];
			for (int k = 0; k < spheres.size(); k++) {
				float t = spheres[k].findIntersection(eyeinit - centerinit, ray);
				//if (t < 0) continue;
				if (t < currentMin && t > 0) {
					current_ambient[0] = spheres[k].color_ambient[0];
					current_ambient[1] = spheres[k].color_ambient[1];
					current_ambient[2] = spheres[k].color_ambient[2];
					currentMin = t;
					type = 1;
				}
			}

			for (int k = 0; k < triangles.size(); k++) {
				float t = triangles[k].findIntersection(eyeinit - centerinit, ray);
				//if (t < 0) continue;
				if (t < currentMin && t > 0) {
					current_ambient[0] = triangles[k].color_ambient[0];
					current_ambient[1] = triangles[k].color_ambient[1];
					current_ambient[2] = triangles[k].color_ambient[2];
					currentMin = t;
					type = 2;
				}
			}

			if (currentMin < 0) {
				//set color for the image to be the ambient color
				color.rgbRed = ambient[0] * (float)255.0;
				color.rgbGreen = ambient[1] * (float)255.0;
				color.rgbBlue = ambient[2] * (float)255.0;
			}
			if (type == 1) {
				//fprintf(stderr, "HITTING A SPHERE\n");
				//set the color to the color of the triangle
				color.rgbRed = current_ambient[0] * (float)255.0;
				color.rgbGreen = current_ambient[1] * (float)255.0;
				color.rgbBlue = current_ambient[2] * (float)255.0;
			} else if (type == 2){
				//fprintf(stderr, "HITTING A TRIANGLE\n");
				color.rgbRed = current_ambient[0] * (float)255.0;
				color.rgbGreen = current_ambient[1] * (float)255.0;
				color.rgbBlue = current_ambient[2] * (float)255.0;
			}
			else if (type == 0) {
				//fprintf(stderr, "HITTING NOTHING\n");
				color.rgbRed = 0;
				color.rgbGreen = 0;
				color.rgbBlue = 0;
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
	float halfHeight = (float)((float)height / ((float)2.0));
	float halfWidth = (float)((float)width / ((float)2.0));
	float fovx_radians = fovx * (pi / float(180.0));
	float fovy_radians = fovy * (pi / float(180.0));
	//fprintf(stderr, "FOVX: [%f], FOVY: [%f]\n", fovx_radians, fovy_radians);
	float alpha = tan(fovx_radians / float(2.0)) * (((float)(j) - halfWidth) / (float)halfWidth);
	float beta = tan(fovy_radians / float(2.0)) * (((float)halfHeight - (float)(i)) / (float)halfHeight);

	//fprintf(stderr, "Alpha: [%f], Beta: [%f]\n", alpha, beta);
	vec3 p1 = (alpha * u) + (beta * v) - w;

	return p1;
}