#version 150

in vec3 inPosition;
in vec3 inNormal;

uniform mat4 myMatrix;
out vec3 v_Color;
out vec3 v_transformedNormal;

uniform mat4 mdlMatrix; // Model to world
uniform mat4 camMatrix; // World to view

void main(void)
{
	mat3 normalMatrix1 = mat3(camMatrix * mdlMatrix * myMatrix);
	v_transformedNormal = normalMatrix1 * inNormal;

	gl_Position = camMatrix * mdlMatrix * myMatrix * vec4(inPosition, 1.0);
	v_Color =  inNormal;
}
