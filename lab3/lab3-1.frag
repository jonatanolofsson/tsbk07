#version 150

out vec4 out_Color;
in vec3 v_Color;
in vec3 v_transformedNormal;
in vec2 v_TexCoord;
uniform float t;
uniform sampler2D texUnit;

void main(void)
{
	const vec3 light = vec3(0.58, 0.58, 0.58);
	float shade = clamp(clamp(dot(normalize(v_transformedNormal), light),0,1) + 0.5, 0, 1);
	//vec4 tmp_Color = texture(texUnit, v_TexCoord);
//	tmp_Color.x = sin(v_TexCoord.y*10 + 2*t)/2 + 0.5;
//	tmp_Color.y = cos(v_TexCoord.y*10 + 2*t)/2 + 0.5;
//	tmp_Color.z = cos(v_TexCoord.x*10 + 2*t)/2 + 0.5;
	//out_Color = tmp_Color*shade;
	out_Color = vec4(shade, shade, shade, 1.0);
}
