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

			//fprintf(stderr, "Intensity: [%f][%f][%f]\n", intensity[0], intensity[1], intensity[2]);

			color.rgbRed = intensity[0] * (float)255.0;
			color.rgbGreen = intensity[1] * (float)255.0;
			color.rgbBlue = intensity[2] * (float)255.0;
			/*Intersection intersection = intersect(ray);

			if (intersection.hit) {
				//if (!isShadowedPixel(intersection.p0, intersection.p1, intersection.currentMin, lights))

				float v = 1.0f; float L = 0.0f;
				float distance = 0.0f; vec3 direction0;
				for (int i = 0; i < lights.size(); i++) {
					if (lights[i].isPoint) {
						//Point Light
						//shadow_ray_direction
						direction0 = normalize(lights[i].light_posn - intersection.point);
						distance = length(direction0);
						L = (1.0f / (attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance)));
						Intersection visiblility = intersect(direction0);
						if (visiblility.hit) {
							fprintf(stderr, "In the shadows! - point light\n");
							v = 0.0f;
						}
					} else {
						//Directional Light
						direction0 = normalize(lights[i].light_posn);
						distance = length(direction0);
						Intersection visiblility = intersect(direction0);
						if (visiblility.hit) {
							fprintf(stderr, "In the shadows! - directional light\n");
							v = 0.0f;
						}
					}

					if (!isInShadow) {
						//trace reflected ray
						vec3 half = normalize(direction0 + eyeinit);

						//vec3 colors = ComputeLight(direction0, lights[i].color, intersection.normal, half, diffuse, specular, shininess);

						//colors += (vec3(intersection.current_ambient[0], intersection.current_ambient[1], intersection.current_ambient[2]) 
							//+ vec3(intersection.object_emission[0], intersection.object_emission[1], intersection.object_emission[2]));

						//colors *= v;

						vec3 diffuse = vec3(intersection.object_diffuse[0], intersection.object_diffuse[1], intersection.object_diffuse[2]) * glm::max(dot(intersection.normal, direction0), 0.f);
						vec3 specular = vec3(intersection.object_specular[0], intersection.object_specular[1], intersection.object_specular[2]) * powf(glm::max(dot(intersection.normal, half), 0.f), shininess);
						vec3 colors = attenuation * lights[i].color * (diffuse + specular);


						color.rgbRed = colors[0] * (float)255.0;
						color.rgbGreen = colors[1] * (float)255.0;
						color.rgbBlue = colors[2] * (float)255.0;
					}
					else {
						//write function for this maybe?
						color.rgbRed = 0.0f;
						color.rgbGreen = 0.0f;
						color.rgbBlue = 0.0f;
					}
				}
			} else {
				color.rgbRed = 0.0f;
				color.rgbGreen = 0.0f;
				color.rgbBlue = 0.0f;
			}*/
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

	//fprintf(stderr, "RET VAL:[%f][%f][%f]\n", retval[0], retval[1], retval[2]);
	return retval;
}

vec3 recursiveRay(vec3 ray, int depth) {
	Intersection intersection = intersect(ray);

	if (!intersection.hit || depth == max_depth) { //BASE CASE
		return vec3(0.0f, 0.0f, 0.0f);
	} else {
		vec3 color;
		float v = 1.0f; float L = 0.0f;
		float distance = 0.0f; vec3 direction0;
		for (int i = 0; i < lights.size(); i++) {
			if (lights[i].isPoint) {
				//Point Light
				//shadow_ray_direction
				direction0 = normalize(lights[i].light_posn - intersection.point);
				distance = length(direction0);
				L = (1.0f / (attenuation.x + (attenuation.y * distance) + (attenuation.z * distance * distance)));
				Intersection visiblility = intersect(direction0);
				if (visiblility.hit) {
					fprintf(stderr, "In the shadows! - point light\n");
					v = 0.0f;
				}
			}
			else {
				//Directional Light
				direction0 = normalize(lights[i].light_posn);
				distance = length(direction0);
				Intersection visiblility = intersect(direction0);
				if (visiblility.hit) {
					fprintf(stderr, "In the shadows! - directional light\n");
					v = 0.0f;
				}
			}
			//trace reflected ray
			vec3 half = normalize(direction0 + eyeinit);

			vec3 colors = ComputeLight(direction0, lights[i].color, intersection.normal, half, diffuse, specular, shininess);

			colors += (vec3(intersection.current_ambient[0], intersection.current_ambient[1], intersection.current_ambient[2]) 
							+ vec3(intersection.object_emission[0], intersection.object_emission[1], intersection.object_emission[2]));

			colors *= v;

			//vec3 diffuse = vec3(intersection.object_diffuse[0], intersection.object_diffuse[1], intersection.object_diffuse[2]) * glm::max(dot(intersection.normal, direction0), 0.f);
			//vec3 specular = vec3(intersection.object_specular[0], intersection.object_specular[1], intersection.object_specular[2]) * powf(glm::max(dot(intersection.normal, half), 0.f), shininess);
			//color = attenuation * lights[i].color * (diffuse + specular);
			color = colors;

			if (intersection.object_specular[0] != 0.0f && intersection.object_specular[1] != 0.0f && intersection.object_specular[2] != 0.0f) {
				vec3 intersection_point = vec3((intersection.p1 * intersection.t) + intersection.p0);
				vec3 reflected_direction = normalize(vec3(intersection.p1) - 2.0f * glm::dot(vec3(intersection.p1), intersection.normal) * intersection.normal);
				return (color + recursiveRay(reflected_direction, depth + 1));
			}

			return color;
		}
	}

	/*Intersection intersection = intersect(ray);
	//fprintf(stderr, "intersection type: %i\n", intersection.type);
	if (intersection.type == 0|| depth == max_depth) { //BASE CASE
		return vec3(0.0f, 0.0f, 0.0f);
	} else if (intersection.type == 1 || intersection.type == 2) {
		fprintf(stderr, "We actually made it here\n");
		vec3 color;
		color[0] = intersection.current_ambient[0];
		color[1] = intersection.current_ambient[1];
		color[2] = intersection.current_ambient[2];
		vec3 intersection_point = vec3((intersection.p1 * intersection.t) + intersection.p0);
		vec3 reflected_direction = normalize(vec3(intersection.p1) - 2.0f * glm::dot(vec3(intersection.p1), intersection.normal) * intersection.normal);
		
		for (int p = 0; p < lights.size(); p++) {
			if (isInShadow(centerinit + intersection.t * ray, lights[p].light_posn)) continue;
			vec3 position = lights[p].light_posn;
			vec3 direction; vec3 half;
			if (!lights[p].isPoint) {
				//Directional Light
				direction = normalize(position);
				half = normalize(direction + eyeinit);
			}
			else {
				//Point Light
				direction = normalize(position - (reflected_direction + intersection_point));
				half = normalize(direction + eyeinit);
			}
			color += ComputeLight(direction, lights[p].color, intersection.normal, half, intersection.object_diffuse, intersection.object_specular, shininess);
		}
		
		return (color + recursiveRay(reflected_direction, depth + 1));
	}*/
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

			intersection.point = vec3(intersection.p0 + (intersection.currentMin * intersection.p1));
			//intersection.point = vec3(transpose(inverse_tranform) * (intersection.p0 + (intersection.currentMin * intersection.p1)));
			vec4 sphereNormal = vec4(intersection.point - spheres[k].sphere_center, 0.0f);
			intersection.normal = normalize(vec3(transpose(inverse_tranform) * sphereNormal));
			intersection.point = vec3(inverse_tranform * vec4(intersection.point, 1.0f));

			//intersection.point = vec3(transpose(inverse_tranform) * (intersection.p0 + (intersection.currentMin * intersection.p1)));
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