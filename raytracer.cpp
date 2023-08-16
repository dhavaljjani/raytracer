#include "raytracer.h"
using namespace std;

void raytracer() {
	FreeImage_Initialise();
	FIBITMAP* bitmap = FreeImage_Allocate(width, height, 24);
	RGBQUAD color;

	for (float i = 0; i < height; i++) {
		for (float j = 0; j < width; j++) {
			fprintf(stderr, "Completed %.1f%%\n\r", 100.0f * ((i * width) + j) / (width * height));
			vec3 ray = RayThruPixel(centerinit, upinit, i + (float)0.5, j + (float)0.5);

			vec3 intensity = recursiveRay(ray, 0);
			
			color.rgbRed = intensity[0] * (float)255.0;
			color.rgbGreen = intensity[1] * (float)255.0;
			color.rgbBlue = intensity[2] * (float)255.0;
			
			FreeImage_SetPixelColor(bitmap, j, height - i, &color);
		}
	}
	input_filename = input_filename.replace(input_filename.find(".test"), input_filename.size(), ".png");
	//fprintf(stderr, input_filename.c_str());
	FreeImage_Save(FIF_PNG, bitmap, input_filename.c_str(), 0);
	FreeImage_DeInitialise();
}



vec3 RayThruPixel(vec3 centerinit, vec3 upinit, float i, float j) {
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

	//float dist = sqrtf(dot(distance, distance)) / sqrtf(dot(direction, direction));
	float dist = sqrtf(pow(distance[0], 2) + pow(distance[1], 2) + pow(distance[2], 2));

	for (int i = 0; i < spheres.size(); i++) {
		Sphere sphe = spheres[i];
		float t = sphe.findIntersection(origin, normalize(lightPos - origin));
		if (t == INFINITY) continue;

		if (t < dist) return true;
	}

	for (int i = 0; i < triangles.size(); i++) {
		Triangle tri = triangles[i];
		float t = tri.findIntersection(origin, normalize(lightPos - origin));
		if (t == INFINITY) continue;

		if (t < dist) return true;
	}

	return false;
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

	//fprintf(stderr, "Compute Light: [%f][%f][%f]\n", retval[0], retval[1], retval[2]);
	return retval;
}

vec3 recursiveRay(vec3 ray, int depth) {
	Intersection intersection = intersect(ray);
	if (depth == max_depth || !intersection.hit) { //BASE CASE
		return vec3(0.0f, 0.0f, 0.0f);
	} else {
		vec3 colors = {0.0f, 0.0f, 0.0f};
		colors[0] += (intersection.current_ambient[0] + intersection.object_emission[0]);
		colors[1] += (intersection.current_ambient[1] + intersection.object_emission[1]);
		colors[2] += (intersection.current_ambient[2] + intersection.object_emission[2]);

		float v = 1.0f; float L = 1.0f;
		float distance = 0.0f; vec3 direction0; 
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i].isPoint) {
				//Point Light
				//shadow_ray_direction
				direction0 = (lights[i].light_posn - intersection.point);
				distance = length(direction0);
				L = (1.0f / (attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance)));
				Intersection visiblility = intersect(direction0);
				if (visiblility.hit) {
					//fprintf(stderr, "In the shadows! - point light\n");
					//v = 0.0f;
				}
				direction0 = normalize(direction0);
			}
			else {
				//Directional Light
				direction0 = normalize(lights[i].light_posn);
				distance = length(direction0);
				Intersection visiblility = intersect(direction0);
				if (visiblility.hit) {
					//fprintf(stderr, "In the shadows! - directional light\n");
					v = 0.0f;
				}
			}

			//trace reflected ray
			vec3 lighting_coeff = { 0.0f, 0.0f, 0.0f };

			vec3 half = normalize(direction0 + eyeinit);

			float nDotL = dot(intersection.normal, direction0);

			lighting_coeff += (lights[i].color * vec3(intersection.object_diffuse[0], intersection.object_diffuse[1], intersection.object_diffuse[2]) * max(nDotL, 0.0f));

			float nDotH = dot(intersection.normal, half);

			lighting_coeff += (vec3(intersection.object_specular[0], intersection.object_specular[1], intersection.object_specular[2]) * pow(max(nDotH, 0.0f), shininess));

			//colors += ComputeLight(direction0, lights[i].color, intersection.normal, half, diffuse, specular, shininess);

			//lighting_coeff *= v;

			lighting_coeff *= L;

			if (isInShadow(vec3(intersection.p0), lights[i].light_posn)) {
				//continue;
				//v = 0.0f;
			}

			lighting_coeff *= v;

			colors += lighting_coeff;

		}

		// + SIr
			// add recursive lighting

			//if (intersection.object_specular[0] != 0.0f && intersection.object_specular[1] != 0.0f && intersection.object_specular[2] != 0.0f) {
		vec3 reflected_direction = normalize((vec3(intersection.p0) - intersection.point) - 2.0f * glm::dot((vec3(intersection.p0) - intersection.point), intersection.normal) * intersection.normal);
		//vec3 reflected_direction = normalize(direction0 - 2.0f * dot(direction0, intersection.normal) * intersection.normal);
		//reflected_direction = normalize(intersection.point + (reflected_direction * shift));
		Intersection reflected_intersection = intersect(reflected_direction);
		//if (reflected_intersection.hit) {
		colors += (vec3(intersection.object_specular[0], intersection.object_specular[1], intersection.object_specular[2]) * recursiveRay(reflected_direction, depth + 1));
		//}
	//}

		return colors;
	}
}

Intersection intersect(vec3 ray) {
	Intersection intersection;
	intersection.currentMin = INFINITY; intersection.t = INFINITY;
	intersection.type = 0; //0 for nothing, 1 for spheres, 2 for triangles
	intersection.hit = false;
	float sphere_min = INFINITY; float triangle_min = INFINITY;
	for (int k = 0; k < spheres.size(); k++) {
		mat4 inverse_tranform = inverse(spheres[k].transform);
		intersection.p0 = inverse_tranform * vec4((eyeinit - centerinit)[0],
			(eyeinit - centerinit)[1], (eyeinit - centerinit)[2], 1.0f);
		intersection.p1 = inverse_tranform * vec4(ray, 0.0f);
		intersection.t = spheres[k].findIntersection(vec3(intersection.p0), vec3(intersection.p1));
		if (intersection.t < 0) continue;
		if (intersection.t < intersection.currentMin && intersection.t > 0) {
			intersection.hit = true;
			intersection.current_ambient[0] = spheres[k].color_ambient[0];
			intersection.current_ambient[1] = spheres[k].color_ambient[1];
			intersection.current_ambient[2] = spheres[k].color_ambient[2];

			intersection.object_emission[0] = spheres[k].object_emission[0];
			intersection.object_emission[1] = spheres[k].object_emission[1];
			intersection.object_emission[2] = spheres[k].object_emission[2];

			intersection.object_diffuse[0] = spheres[k].object_diffuse[0];
			intersection.object_diffuse[1] = spheres[k].object_diffuse[1];
			intersection.object_diffuse[2] = spheres[k].object_diffuse[2];

			intersection.object_specular[0] = spheres[k].object_specular[0];
			intersection.object_specular[1] = spheres[k].object_specular[1];
			intersection.object_specular[2] = spheres[k].object_specular[2];

			sphere_min = intersection.t;
			intersection.currentMin = sphere_min;
			intersection.type = 1;

			/*intersection.point = vec3(intersection.p0 + (intersection.currentMin * intersection.p1));
			vec4 sphereNormal = vec4(intersection.point - spheres[k].sphere_center, 0.0f);
			intersection.normal = normalize(vec3(transpose(inverse_tranform) * sphereNormal));
			intersection.point = vec3(inverse_tranform * vec4(intersection.point, 1.0f));*/

			vec3 pT = vec3(intersection.p0 + (intersection.t * intersection.p1));
			vec4 p = spheres[k].transform * vec4(pT, 1);
			vec4 normalSphere = vec4(pT - spheres[k].sphere_center, 0.0f);
			intersection.point = vec3(p / p.w);
			intersection.normal = normalize(vec3(transpose(inverse_tranform) * normalSphere));
		}
	}
	for (int k = 0; k < triangles.size(); k++) {
		mat4 inverse_tranform = inverse(triangles[k].transform);
		intersection.p0 = inverse_tranform * vec4((eyeinit - centerinit)[0],
			(eyeinit - centerinit)[1], (eyeinit - centerinit)[2], 1.0f);
		intersection.p1 = inverse_tranform * vec4(ray, 0.0f);
		intersection.t = triangles[k].findIntersection(vec3(intersection.p0), vec3(intersection.p1));
		if (intersection.t < 0) continue;
		if (intersection.t < intersection.currentMin && intersection.t > 0) {
			intersection.hit = true;

			intersection.current_ambient[0] = triangles[k].color_ambient[0];
			intersection.current_ambient[1] = triangles[k].color_ambient[1];
			intersection.current_ambient[2] = triangles[k].color_ambient[2];

			intersection.object_emission[0] = triangles[k].object_emission[0];
			intersection.object_emission[1] = triangles[k].object_emission[1];
			intersection.object_emission[2] = triangles[k].object_emission[2];

			intersection.object_diffuse[0] = triangles[k].object_diffuse[0];
			intersection.object_diffuse[1] = triangles[k].object_diffuse[1];
			intersection.object_diffuse[2] = triangles[k].object_diffuse[2];

			intersection.object_specular[0] = triangles[k].object_specular[0];
			intersection.object_specular[1] = triangles[k].object_specular[1];
			intersection.object_specular[2] = triangles[k].object_specular[2];


			triangle_min = intersection.t;
			intersection.currentMin = triangle_min;
			intersection.type = 2;

			Triangle tri = triangles[k];

			intersection.point = vec3(intersection.p0 + (intersection.currentMin * intersection.p1));
			vec3 A = vec3(vec4(tri.A[0], tri.A[1], tri.A[2], 1.0f));
			vec3 B = vec3(vec4(tri.B[0], tri.B[1], tri.B[2], 1.0f));
			vec3 C = vec3(vec4(tri.C[0], tri.C[1], tri.C[2], 1.0f));
			intersection.normal = vec3(normalize(transpose(inverse_tranform) * vec4(cross(C - A, B - A), 0.0f)));

			
			//vec3 A = vec3(inverse_tranform * vec4(tri.A[0], tri.A[1], tri.A[2], 1.0f));
			//vec3 B = vec3(inverse_tranform * vec4(tri.B[0], tri.B[1], tri.B[2], 1.0f));
			//vec3 C = vec3(inverse_tranform * vec4(tri.C[0], tri.C[1], tri.C[2], 1.0f));
			//intersection.normal = normalize(cross(C - A, B - A));
			intersection.point = vec3(inverse_tranform * vec4(intersection.point, 1.0f)); //figure this out!!
		}
	}
	intersection.currentMin = min(sphere_min, triangle_min);
	return intersection;
}