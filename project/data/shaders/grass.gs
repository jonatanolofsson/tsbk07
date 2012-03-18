#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 16) out;

uniform mat4 baseMatrix;

out vec4 gPosition;
smooth out vec2 texPosition;
 
void main(){
    vec4 seedPos = gl_in[0].gl_Position;
    
    vec4 width = vec4(0.1, 0.0, 0.0, 0.0);

    float height = 0.2;
    float windforce = 0.05;
    int lod = 3;
    for(int i = 0; i <= lod; i++) {
        vec4 windOffset = (i/float(lod))*vec4(windforce, 0.0, -0.03, 0.0);
        vec4 heightOffset = (i/float(lod))*vec4(0.0, height, 0.0, 0.0);
        
        // First side
        gl_Position = gPosition = baseMatrix * (seedPos + heightOffset + windOffset);
        texPosition = vec2(0, i/float(lod));
        EmitVertex();
        
        // Other side (first side + width)
        gl_Position = gPosition = baseMatrix * (seedPos + heightOffset + width + windOffset);
        texPosition = vec2(1.0, i/float(lod));
        EmitVertex();
    }
    EndPrimitive();
}
