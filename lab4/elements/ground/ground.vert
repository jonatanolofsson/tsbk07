#version 150

in vec3 inPosition;
in vec2 inTexCoord;

out vec2 v_TexCoord;

uniform mat4 projectionMatrix; // World to screen
uniform mat4 baseMatrix; // To world
uniform vec3 camera_position;

void main(void)
{
    gl_Position = projectionMatrix * baseMatrix * vec4(inPosition, 1.0);
    v_TexCoord = vec2(inTexCoord.s + camera_position[0]/2, inTexCoord.t - camera_position[2]/2);
}
