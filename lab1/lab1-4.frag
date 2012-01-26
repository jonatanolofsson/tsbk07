#version 150

out vec4 out_Color;
in vec3 v_Color;

void main(void)
{
	out_Color = vec4(v_Color, 1.0);
}
