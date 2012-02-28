#version 150

in vec3 inPosition;
in vec3 inNormal;
in vec2 inTexCoord;

out vec2 v_TexCoord;

uniform mat4 projectionMatrix; // World to screen
uniform mat4 baseMatrix; // World to view

void main(void)
{
    gl_Position =  projectionMatrix * mat4(mat3(baseMatrix)) * vec4( inPosition, 1.0);
    v_TexCoord = inTexCoord;
}
