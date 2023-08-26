![teapot](https://github.com/dhavaljjani/raytracer/blob/main/teapot.gif)

# raytracer

A simple raytracer made with pure C++ and glm functions. Can render primitive objects such as spheres and triangles with vertices, using a .test input file.
Handles recursive lighting with specular, diffuse, ambient, and emission lighting. **In Progress**

Uses input .test files where triangles, spheres, lights, and lighting values can be specified.

## Images

Rendering of dice:

![Rendering of dice](https://github.com/dhavaljjani/raytracer/blob/main/scene2.png)

Rendering of table (no lighting):

![Rendering of table](https://github.com/dhavaljjani/raytracer/blob/main/scene3.png)

Rendering of table (to showcase diffuse lighting):

![Rendering of table (diffuse)](https://github.com/dhavaljjani/raytracer/blob/main/scene4-diffuse.png)

Rendering of table (to showcase specular lighting):

![Rendering of table (specular)](https://github.com/dhavaljjani/raytracer/blob/main/scene4-specular.png)

Rendering of spheres besides each other:

![Rendering of spheres](https://github.com/dhavaljjani/raytracer/blob/main/scene5.png)

Rendering of a simple Cornell box:

![Rendering of cornell box](https://github.com/dhavaljjani/raytracer/blob/main/scene6.png)

Rendering of the Stanford dragon:

![Rendering of dragon](https://github.com/dhavaljjani/raytracer/blob/main/scene7.png)

## How does a raytracer work?

A raytracer is essentially a way to render objects using linear algebra. What we can imagine is a camera situated somewhere in a 3D coordinate space. It points in a certain direction, and creates a virtual 2D image based on where it points at. Looking at this following image provides some more context:

![image](https://github.com/dhavaljjani/raytracer/assets/56317794/9111dc7c-eb69-4eec-b293-db714f9237b5)

Through each pixel of the image, the camera shoots a ray towards the world, and calculates if it hits an object or not (either a triangle or sphere). If so, the color of surface is added to the pixel. Next, a ray is shot from that point of impact to every light in the world. If there is something in between these two, we can assume this point in the world is shadowed. We assume two types of lights to exist, point lights originating at a specific point, or directional lights with originates from a general direction and impacts all objects that are parallel. 

In order to handle precision, we also use a shift when shooting rays, to shoot them slightly away from the objects they originate from, so that they don't detect themselves.

If the point is not shadowed, we can then begin to draw the reflective ray from the point to where light would bounce, and we can keep doing this indefinitely, or until we want (by defining a max depth). We use the following equation for the lighting in general, which makes use of Phong and Lambert shading techniques. This defines the intensity of light at every pixel in terms of the RGB values. Not shown but what is also included and vital is a "+ SI<sub>R</sub>" which is specular lighting multiplied by the intensity of the recursive ray, which potentially spawns even more reflective recursive rays, and so on.

![image](https://github.com/dhavaljjani/raytracer/assets/56317794/f7612d11-9da4-4d67-a5d1-0d32c92ba4a9)

A = ambient, E = emission, Vi = 1 or 0 depending on if point is shadowed, D = diffuse, N = intersection normal, S = specular, s = shininess

## Some more images!

Rendering of Indra's Net:

![Rendering of indras net 2](https://github.com/dhavaljjani/raytracer/blob/main/indras2.png)

![Rendering of indras net](https://github.com/dhavaljjani/raytracer/blob/main/indras.png)

Rendering of a marble pyramid/desert scene:

![Rendering of dragon](https://github.com/dhavaljjani/raytracer/blob/main/drydryruins.png)

Rendering of some random spheres:

![Rendering of spheres](https://github.com/dhavaljjani/raytracer/blob/main/spheres.png)

Rendering of a Rubik's Cube:

![Rendering of cube](https://github.com/dhavaljjani/raytracer/blob/main/rubikscube.png)

Rendering of a UFO:

![Rendering of cube](https://github.com/dhavaljjani/raytracer/blob/main/ufo.png)

