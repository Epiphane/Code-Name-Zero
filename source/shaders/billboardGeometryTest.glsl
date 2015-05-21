#version 330

layout(points) in;
layout(triangle_strip) out;
layout(max_vertices = 4) out;

uniform mat4 gProjection;
uniform mat4 gView;
uniform vec3 gCameraPos;
uniform float gBillboardSize;

out vec2 TexCoord;

void main()
{
    vec3 Pos = gl_in[0].gl_Position.xyz; // position of the particle in world space
    
    vec4 clip = gProjection * gView * vec4(Pos, 1.0);
    float invWidth = 1.0/1024;
    float invHeight= 1.0/768;
    int iconSize=16;
    
    // Used to unproject the XY plane
    float dx = iconSize * invWidth * 0.5 * clip.w;
    float dy = iconSize * invHeight * 0.5 * clip.w;
    dx *= gBillboardSize * clip.w;
    dy *= gBillboardSize * clip.w;
    
    vec3 temp = gCameraPos; // not using gCameraPos
    
    
    
    // Bottom left corner of the quad
    gl_Position = vec4(clip.x - dx, clip.y + dy, clip.z, clip.w);
    TexCoord = vec2(0.0, 0.0);
    EmitVertex();
    
    // Upper left corner of the quad
    gl_Position = vec4(clip.x - dx, clip.y - dy, clip.z, clip.w);
    TexCoord = vec2(0.0, 1.0);
    EmitVertex();
    
    // Bottom right corner of the quad
    gl_Position = vec4(clip.x + dx, clip.y + dy, clip.z, clip.w);
    TexCoord = vec2(1.0, 0.0);
    EmitVertex();
    
    // Make the second triangle by creating a vertex at upper right corner
    gl_Position = vec4(clip.x + dx, clip.y - dy, clip.z, clip.w);
    TexCoord = vec2(1.0, 1.0);
    EmitVertex();
    
    EndPrimitive();
}

