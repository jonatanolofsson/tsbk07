#version 150

in vec4 gPosition;
smooth in vec2 texPosition;

uniform sampler2D texUnit;
uniform sampler2D maskUnit;

out vec4 outColor;

void main(){    
    outColor = vec4(0.0, 1.0, 0.0, 1.0);
}
