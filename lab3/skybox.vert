#version 150

in vec3 inPosition;
in vec2 inTexCoord;

out vec2 v_TexCoord;

uniform mat4 projectionMatrix; // World to screen
uniform mat4 baseMatrix; // World to view
uniform mat4 camMatrix; // World to view

void main(void)
{
    mat4 camFixed = mat4(1.0);
    camFixed[3][0] = camMatrix[3][0];
    camFixed[3][2] = -camMatrix[3][2];
	gl_Position = projectionMatrix * camMatrix * camFixed * vec4(inPosition, 1.0);
	v_TexCoord = vec2(inTexCoord.s + camMatrix[3][0]/2, inTexCoord.t + camMatrix[3][2]/2);
}
