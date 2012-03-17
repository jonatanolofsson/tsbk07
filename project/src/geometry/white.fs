#version 150

in vec4 gPosition;
smooth in vec2 texPosition;

uniform sampler2D texUnit;
uniform sampler2D maskUnit;

out vec4 outColor;

void main(){    
    outColor = texture(maskUnit, texPosition).x * texture(texUnit, texPosition);
    //outColor = vec4(0.0, 0.0, 0.5, 0.5);
}
