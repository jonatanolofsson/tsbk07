#version 150

in vec3 seedPosition;

void main() {
	gl_Position = vec4(seedPosition, 1.0);
}
