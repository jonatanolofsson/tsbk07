#version 150
#extension GL_ARB_separate_shader_objects : enable

in vec3 inPosition;
uniform mat4 projMatrix;
uniform mat4 baseMatrix;
out vec3 vPosition;

void main() {
    vPosition = inPosition;
	gl_Position = baseMatrix * vec4(inPosition, 1.0);
}

