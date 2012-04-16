#version 150

in vec3 inPosition;
in vec2 inTexCoord;

out vec2 v_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 baseMatrix;
uniform mat4 cameraMatrix;


void main(void)
{
    vec3 cameraPosition = vec3(-cameraMatrix[0][3], 0.0, cameraMatrix[2][3]);
    v_TexCoord = vec2(inTexCoord.s + cameraPosition.x/2, inTexCoord.t - cameraPosition.z/2);
    gl_Position = projectionMatrix * cameraMatrix * baseMatrix * vec4(inPosition, 1.0);
	
}
