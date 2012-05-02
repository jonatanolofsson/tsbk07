#version 150

layout(points) in;
layout(triangle_strip, max_vertices = 32) out;

uniform mat4 projectionMatrix;
uniform mat4 baseMatrix;
uniform mat4 cameraMatrix;
uniform vec3 cameraPosition;

uniform vec3 windforce;
uniform float time;

out vec4 gPosition;
out vec3 gNormal;
smooth out vec2 texPosition;

out float personalRand;
float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main(){
    // Camera distance
    float cameraDistance = length(vec4(cameraPosition.x, 0.0, cameraPosition.z, 0.0) - gl_in[0].gl_Position);

    // Constants
    float height = clamp(3 - cameraDistance/20, 0.5, 3);
    float bladeWidth = clamp(0.01 + cameraDistance/20, 0.01, 0.2);
    int lod = clamp(int(round(1 / pow(cameraDistance/10, 2))), 1, 4);
    mat4 totalMatrix = projectionMatrix * baseMatrix;//cameraMatrix;


    // Personalize our blade of grass
    vec4 seedPos = gl_in[0].gl_Position;
    float personal = sin(1.8*fract(seedPos.x) + 1.6*fract(seedPos.z));

    // Wind
    vec3 windforce_local = windforce;
    windforce_local.x = sin(time + rand(seedPos.xz));
    vec4 windVec = vec4(0.2*windforce_local, 0.0);

    // Calculations based on personalization
    vec4 width = 0.5*bladeWidth*normalize(vec4(personal, 0.0, personal, 0.0));
    vec4 heightVec = (1 + personal/3) * vec4(0.0, height, 0.0, 0.0);
    vec4 Ydisplacement = 0.5*length(windVec)*heightVec;

    // Side one
    for(int i = 0; i <= lod; i++) {
        float progress = (i/float(lod));
        float next = (i+1/float(lod));
        vec4 windOffset =  personal*(progress*progress) * windVec;
        // Pos.y = Pos.y - len(windVec) * windCoeff * 0.5
        vec4 heightOffset = progress*heightVec - (progress*progress)*Ydisplacement;

        // Plus one vector used for normals
        vec3 nextUp = vec3((next*next)*windVec + (next*heightVec - (next*next)*Ydisplacement));

        // Middle point
        vec4 firstSide = (seedPos + heightOffset + windOffset);
        gl_Position = gPosition = totalMatrix * firstSide;
        texPosition = vec2(0.5, progress);
        personalRand = personal + progress/10.0;
        gNormal = normalize(cross(vec3(width), nextUp));
        EmitVertex();

        // Right point (first side + width)
        gl_Position = gPosition = totalMatrix * (firstSide + width);
        texPosition = vec2(1.0, progress);
        personalRand = personal;
        gNormal = normalize(cross(nextUp, vec3(-1*width)));
        EmitVertex();
    }
    EndPrimitive();

    // Side two
    width = 0.5*bladeWidth*normalize(vec4(-personal, 0.0, personal, 0.0));
    for(int i = 0; i <= lod; i++) {
        float progress = (i/float(lod));
        float next = (i+1/float(lod));
        vec4 windOffset =  personal*(progress*progress) * windVec;
        // Pos.y = Pos.y - len(windVec) * windCoeff * 0.5
        vec4 heightOffset = progress*heightVec - (progress*progress)*Ydisplacement;

        // Plus one vector used for normals
        vec3 nextUp = vec3((next*next)*windVec + (next*heightVec - (next*next)*Ydisplacement));

        // Middle point
        vec4 firstSide = (seedPos + heightOffset + windOffset);
        gl_Position = gPosition = totalMatrix * firstSide;
        texPosition = vec2(0.5, progress);
        personalRand = personal + progress/10.0;
        gNormal = normalize(cross(nextUp, vec3(-1*width)));
        EmitVertex();

        // Left point (first side -width)
        gl_Position = gPosition = totalMatrix * (firstSide - width);
        texPosition = vec2(0.0, progress);
        personalRand = personal;
        gNormal = normalize(cross(nextUp, vec3(width)));
        EmitVertex();
    }
    EndPrimitive();
}

