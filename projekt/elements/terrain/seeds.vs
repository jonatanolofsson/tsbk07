#version 150

in vec3 seedPosition;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main() {
    float r = rand(seedPosition.xz)*2 - 1;
    float q = rand(vec2(3.324, r))*2 - 1;
    gl_Position = vec4(seedPosition + vec3(r,0,q)/10, 1.0);
}
