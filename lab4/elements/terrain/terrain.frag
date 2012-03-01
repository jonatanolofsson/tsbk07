#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 transformedNormal;
uniform sampler2D tex;

void main(void)
{

    const vec3 light = vec3(0.58, 0.58, 0.58);
    float shade = clamp(dot(normalize(transformedNormal), light), 0, 1);
    outColor = vec4(vec3(texture(tex, texCoord))*shade, 1.0);
    //~ outColor = vec4(transformedNormal, 1.0);
}
