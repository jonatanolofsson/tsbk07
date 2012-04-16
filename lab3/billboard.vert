#version 150

in vec3 inPosition;
in vec2 inTexCoord;

out vec2 v_TexCoord;

uniform mat4 projectionMatrix; // World to screen
uniform mat4 baseMatrix; // World to view
uniform mat4 camMatrix; // World to view
uniform vec3 camera_xz_position;

void main(void)
{

    gl_Position = projectionMatrix * camMatrix *  vec4(inPosition + camera_xz_position, 1.0);
	v_TexCoord = vec2(inTexCoord.s + camera_xz_position[0]/2, inTexCoord.t - camera_xz_position[2]/2);
}
