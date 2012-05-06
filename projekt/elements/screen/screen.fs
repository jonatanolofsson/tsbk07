#version 150

out vec4 out_Color;

in vec2 v_TexCoord;
uniform sampler2D texUnit;
uniform sampler2D depthUnit;
uniform sampler2D zUnit;
uniform sampler2D blurrUnit;

uniform mat4 projectionMatrix;
uniform vec3 cameraPosition;

void main(void) {
    vec4 fragmentPos = texture(depthUnit, v_TexCoord);
    
    float z = -projectionMatrix[2][3] / (texture(zUnit, v_TexCoord).z + projectionMatrix[2][2]);

    float integral = 1 * -(exp(-cameraPosition.y*0.1) - exp(-fragmentPos.y*0.4));
    
    float F = integral * z / (fragmentPos.y - cameraPosition.y);
    float alpha = 1.0 - exp(-F);
    
    vec4 fogColor = texture(zUnit, v_TexCoord);
    //vec4 terrainColor = mix(texture(texUnit, v_TexCoord), texture(blurrUnit, v_TexCoord), 2*alpha);
    vec4 terrainColor = texture(blurrUnit, v_TexCoord);
    out_Color = mix(terrainColor, fogColor, alpha);
    //out_Color = texture(texUnit, v_TexCoord);
}
