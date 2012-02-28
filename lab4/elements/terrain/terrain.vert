#version 150

in  vec3 inPosition;
in  vec3 inNormal;
in vec2 inTexCoord;
out vec2 texCoord;

// NY
uniform mat4 projectionMatrix;
uniform mat4 baseMatrix;

void main(void)
{
    mat3 normalMatrix1 = mat3(baseMatrix);
    texCoord = inTexCoord;
    gl_Position = projectionMatrix * baseMatrix * vec4(inPosition, 1.0);
}
