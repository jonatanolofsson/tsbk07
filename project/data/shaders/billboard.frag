#version 330 core

layout(location = 0) out vec4 out_Color;

in vec2 v_TexCoord;
uniform sampler2D texUnit;

void main(void) {
	out_Color = texture(texUnit, v_TexCoord);
}
