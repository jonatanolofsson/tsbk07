#version 150
#extension GL_ARB_separate_shader_objects : enable

in vec3 inPosition;
in vec2 inTexCoord;

uniform mat4 projectionMatrix;
uniform mat4 baseMatrix;

out vec2 v_TexCoord;
out vec3 vPosition;

void main() {
    vPosition = inPosition;
    v_TexCoord = inTexCoord;
    gl_Position = baseMatrix * vec4(inPosition, 1.0);

}

