#version 150

out vec4 out_Color;

in vec2 v_TexCoord;
uniform sampler2D texUnit;
uniform sampler2D depthUnit;

void main(void) {
    vec4 fogColor = texture(depthUnit, v_TexCoord);
    out_Color = mix(texture(texUnit, v_TexCoord), fogColor, 1.0);
}
