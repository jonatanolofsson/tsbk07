#version 150

out vec4 out_Color;

in vec3 v_Color;
in vec3 v_transformedNormal;
in vec2 v_TexCoord;
in vec3 current_position;

uniform sampler2D firstTexUnit;
uniform sampler2D secondTexUnit;

// Lighting
uniform mat4 lightSourcesColors;
uniform mat4 lightSourcesDirections;
uniform vec3 camera_position;

void main(void)
{
    vec3 n = normalize(v_transformedNormal);
	float ambientLight = 0.2;
	vec3 color = vec3(ambientLight);
	vec3 l;
	for(int i=0; i < 4; i++) {
	    if(lightSourcesDirections[i][3] > 0.5) { // Directional light
            l = normalize(-vec3(lightSourcesDirections[i]));
	    } else { // Positional light
            l = normalize(vec3(lightSourcesDirections[i]) - current_position);
	    }
        vec3 R = normalize(2 * n * dot(n,l) - l);

        vec3 v = normalize(camera_position - current_position);
        float cosphi = clamp(dot(R, v), 0, 1);

        color += vec3(lightSourcesColors[i])*pow(cosphi, lightSourcesColors[i][3]);
	}
	vec4 firstTexture = texture(firstTexUnit, v_TexCoord);
	vec4 secondTexture = texture(secondTexUnit, v_TexCoord);

    out_Color = vec4(color, 1.0) * mix(firstTexture, secondTexture, 0.2); //+ vec4(ambient, ambient, ambient, 1.0);
}
