#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;


out vec3 v_Color;
out vec3 v_transformedNormal;
out vec2 v_TexCoord;
out vec3 current_position;

uniform mat4 projectionMatrix; // World to screen
uniform mat4 mdlMatrix; // Base to world
uniform mat4 baseMatrix; // Model to base
uniform mat4 camMatrix; // World to view

// Lighting
uniform mat4 lightSourcesColors;
uniform mat4 lightSourcesDirections;

void main(void)
{
	v_transformedNormal = mat3(baseMatrix * mdlMatrix) * inNormal;

	gl_Position = projectionMatrix * camMatrix * baseMatrix * mdlMatrix * vec4(inPosition, 1.0);
	v_Color =  inNormal;
	current_position = vec3(baseMatrix * mdlMatrix * vec4(inPosition, 1.0));
	float phi = atan2(inNormal[2], inNormal[0]);
	const float R = 10;
	v_TexCoord = vec2(R*phi/3.14, current_position[1]);
}

