#version 330 core

in vec4 gPosition;
in vec3 gNormal;
smooth in vec2 texPosition;

uniform sampler2D texUnit;
uniform sampler2D maskUnit;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec4 outDepth;
in float personalRand;

void main(){

    const vec3 light = vec3(0.58, 0.58, 0.58);
    float shade = clamp(dot(gNormal, light), 0.4, 1.0);

    // Green grass color
    vec4 grassColor = texture(texUnit, texPosition);
    
    // Alpha Masking
	float blend = 255 * texture(maskUnit, texPosition).x;    
    float alpha = (1 - blend) * (grassColor.a - 0.5) + 0.5;

    // Orange tint
    vec4 orange = vec4(0.8, 0.5, 0.0, 1.0);
    
    // Darker towards bottom
    vec3 darkerColor = vec3(texPosition.y * vec3(grassColor));

    // Final write check
    if(alpha < 0.3) discard;
    outColor = 2*texPosition.y*shade*mix(orange, grassColor, personalRand);
    outDepth = gPosition;
}
