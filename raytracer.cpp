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

	for (float i = 0; i < height; i++) {
		for (float j = 0; j < width; j++) {
			vec3 ray = RayThruPixel(centerinit, upinit, i + (float)0.5, j + (float)0.5);
			float currentMin = 1000000.0;
			vec3 intensity(0.0f, 0.0f, 0.0f);

			int type = 0; //0 if no intersect, 1 if sphere, 2 if triangle
			float current_ambient[3];
			for (int k = 0; k < spheres.size(); k++) {
				float t = spheres[k].findIntersection(eyeinit - centerinit, ray);
				//if (t < 0) continue;
				if (t < currentMin && t > 0) {
					current_ambient[0] = spheres[k].color_ambient[0];
					current_ambient[1] = spheres[k].color_ambient[1];
					current_ambient[2] = spheres[k].color_ambient[2];
					intensity[0] = current_ambient[0];
					intensity[1] = current_ambient[1];
					intensity[2] = current_ambient[2];
					currentMin = t;
					type = 1;
					vec3 ray_direction = normalize(ray);

					vec3 cross_ray = (ray_direction * t) - eyeinit;
					vec3 normal = cross(cross_ray, spheres[k].sphere_center);
					//fprintf(stderr, "N:[%f][%f][%f]\n", normal[0], normal[1], normal[2]);
					for (int p = 0; p < lights.size(); p++) {
						if (isInShadow(centerinit + t * ray, lights[p].light_posn)) continue;
						if (!lights[p].isPoint) {
							//Directional Light
							fprintf(stderr, "DIRECTIONAL\n");
							vec3 direction0 = normalize(vec3(lights[p].light_posn[0], lights[p].light_posn[1], lights[p].light_posn[2]));
							vec3 half0 = normalize(direction0 + eyeinit);
							intensity += ComputeLight(direction0, lights[p].color, normal, half0, diffuse, specular, shininess);
						}
						else {
							//Point Light
							fprintf(stderr, "POINT\n");
							vec3 position = lights[p].light_posn;
							vec3 direction1 = normalize(position - ray);
							vec3 half1 = normalize(direction1 + eyeinit);
							intensity += ComputeLight(direction1, lights[p].color, normal, half1, diffuse, specular, shininess);
						}
						//intensity[0] = current_ambient[0];
						//intensity[1] = current_ambient[1];
						//intensity[2] = current_ambient[2];
					}
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
					intensity[0] = current_ambient[0];
					intensity[1] = current_ambient[1];
					intensity[2] = current_ambient[2];
					vec3 ray_direction = normalize(ray);
					Triangle tri = triangles[k];


					vec3 cross_ray = (ray_direction * t) - eyeinit;
					vec3 normal = cross(tri.B - tri.A, tri.C - tri.A);
					//fprintf(stderr, "N:[%f][%f][%f]\n", normal[0], normal[1], normal[2]);
					for (int p = 0; p < lights.size(); p++) {
						if (isInShadow(centerinit + t * ray, lights[p].light_posn)) continue;
						if (!lights[p].isPoint) {
							//Directional Light
							fprintf(stderr, "DIRECTIONAL\n");
							vec3 direction0 = normalize(vec3(lights[p].light_posn[0], lights[p].light_posn[1], lights[p].light_posn[2]));
							vec3 half0 = normalize(direction0 + eyeinit);
							intensity += ComputeLight(direction0, lights[p].color, normal, half0, diffuse, specular, shininess);
						}
						else {
							//Point Light
							fprintf(stderr, "POINT\n");
							vec3 position = lights[p].light_posn;
							vec3 direction1 = normalize(position - ray);
							vec3 half1 = normalize(direction1 + eyeinit);
							intensity += ComputeLight(direction1, lights[p].color, normal, half1, diffuse, specular, shininess);
						}
						//intensity[0] = current_ambient[0];
						//intensity[1] = current_ambient[1];
						//intensity[2] = current_ambient[2];
					}
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
				color.rgbRed = intensity[0] * 255.0;
				color.rgbGreen = intensity[1] * 255.0;
				color.rgbBlue = intensity[2] * 255.0;
			}
			else if (type == 2) {
				//fprintf(stderr, "HITTING A TRIANGLE\n");
				color.rgbRed = intensity[0] * 255.0;
				color.rgbGreen = intensity[1] * 255.0;
				color.rgbBlue = intensity[2] * 255.0;
			}
			else if (type == 0) {
				//fprintf(stderr, "HITTING NOTHING\n");
				color.rgbRed = 0;
				color.rgbGreen = 0;
				color.rgbBlue = 0;
			}
			FreeImage_SetPixelColor(bitmap, j, height - i, &color);
		}
	}
	input_filename = input_filename.replace(input_filename.find(".test"), input_filename.size(), ".png");
	//fprintf(stderr, input_filename.c_str());
	FreeImage_Save(FIF_PNG, bitmap, input_filename.c_str(), 0);
	FreeImage_DeInitialise();
}

vec3 RayThruPixel(vec3 centeri, vec3 upi, float i, float j) {
	vec3 w = normalize(eyeinit - centerinit);
	vec3 u = normalize(cross(upinit, w));
	vec3 v = cross(w, u);
	float halfHeight = (float)((float)height / ((float)2.0));
	float halfWidth = (float)((float)width / ((float)2.0));
	float fovx_radians = fovx * (pi / float(180.0));
	float fovy_radians = fovy * (pi / float(180.0));
	//fprintf(stderr, "FOVX: [%f], FOVY: [%f]\n", fovx_radians, fovy_radians);
	float alpha = tan(fovx_radians / float(2.0)) * (((j)-halfWidth) / halfWidth);
	float beta = tan(fovy_radians / float(2.0)) * (((float)halfHeight - (float)(i)) / (float)halfHeight);

	//fprintf(stderr, "Alpha: [%f], Beta: [%f]\n", alpha, beta);
	vec3 p1 = (alpha * u) + (beta * v) - w;

	return p1;
}

bool isInShadow(vec3 origin, vec3 lightPos) {
	vec3 distance = lightPos - origin;
	vec3 direction = normalize(lightPos - origin);

	float dist = sqrtf(dot(distance, distance)) / sqrtf(dot(direction, direction));

	for (int i = 0; i < spheres.size(); i++) {
		Sphere sphe = spheres[i];
		float t = sphe.findIntersection(origin, direction);
		if (t < 0) continue;

		if (t < dist) return true;
	}

	for (int i = 0; i < triangles.size(); i++) {
		Triangle tri = triangles[i];
		float t = tri.findIntersection(origin, direction);
		if (t < 0) continue;

		if (t < dist) return true;
	}

	return false;
}

vec3 getColor(int type, int index, vec3 position, int depth) {
	if (type == 1) { //intersection with a sphere
		Sphere sphe = spheres[index];
		vec3 I(sphe.color_ambient[0], sphe.color_ambient[1], sphe.color_ambient[2]);

		for (int i = 0; i < lights.size(); i++) {
			//if the point is shadowed, don't add that light
			if (isInShadow(position, lights[i].light_posn)) continue;

			vec3 L = lights[i].light_posn;
		}


	}

	if (type == 2) {//intersection with a triangle
		Triangle tri = triangles[index];
		vec3 I(tri.color_ambient[0], tri.color_ambient[1], tri.color_ambient[2]);

		for (int i = 0; i < lights.size(); i++) {
			if (isInShadow(position, lights[i].light_posn)) continue;

			vec3 L = lights[i].light_posn;
		}
	}

	return vec3(0.0, 0.0, 0.0);//should never reach this case
}

vec3 ComputeLight(const vec3 direction, const vec3 lightcolor, const vec3 normal, const vec3 halfvec, const float mydiffuse[3], const float myspecular[3], const float myshininess) {
	//Mostly taken from hw 2
	float nDotL = dot(normal, direction);
	vec3 lambert;
	lambert[0] = mydiffuse[0] * max(nDotL, 0.0f);
	lambert[1] = mydiffuse[1] * max(nDotL, 0.0f);
	lambert[2] = mydiffuse[1] * max(nDotL, 0.0f);

	float nDotH = dot(normal, halfvec);
	vec3 phong;
	phong[0] = myspecular[0] * pow(max(nDotH, 0.0f), myshininess);
	phong[1] = myspecular[1] * pow(max(nDotH, 0.0f), myshininess);
	phong[2] = myspecular[2] * pow(max(nDotH, 0.0f), myshininess);

	vec3 retval;
	retval[0] = (lightcolor[0] * (lambert[0] + phong[0]));
	retval[1] = (lightcolor[1] * (lambert[1] + phong[1]));
	retval[2] = (lightcolor[2] * (lambert[2] + phong[2]));

	fprintf(stderr, "RET VAL:[%f][%f][%f]n", retval[0], retval[1], retval[2]);
	return retval;
}