#version 150

in vec3 inPosition;
in vec3 inNormal;

uniform mat4 myMatrix;
out vec3 v_Color;

void main(void)
{
	gl_Position = vec4(inPosition, 1.0) * myMatrix;
	v_Color =  inNormal;
}
