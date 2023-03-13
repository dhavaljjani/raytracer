#version 330 core
// Do not use any version older than 330! Modern OpenGL will break!

// Inputs to the vertex shader
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

// Uniform variables
uniform mat4 modelview;
uniform mat4 projection;

// Additional outputs for the vertex shader in addition to gl_Position
out vec3 mynormal;
out vec4 myvertex;

void main() {
    gl_Position = projection * modelview * vec4(position, 1.0f); 
	// Forward these vectors to the fragment shader
	mynormal = normal;
	myvertex = vec4(position, 1.0f);
}

