#version 150

out vec4 out_Color;

in vec2 v_TexCoord;
uniform sampler2D texUnit;

void main(void)
{
	out_Color = texture(texUnit, v_TexCoord);
	//out_Color = vec4(v_TexCoord.s, 1.0, v_TexCoord.t, 1.0);
	//out_Color = vec4(1.0);
}
