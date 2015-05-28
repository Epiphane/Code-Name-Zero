#version 330

layout(points) in;
layout(points) out;
layout(max_vertices = 40) out;

in float Type0[];
in vec3 Position0[];
in vec3 Velocity0[];
in float Age0[];

out float Type1;
out vec3 Position1;
out vec3 Velocity1;
out float Age1;

uniform float gDeltaTimeMillis;
uniform float gTime;
uniform sampler1D gRandomTexture;
uniform float gLauncherLifetime;
uniform float gShellLifetime;
uniform float gPlayerSpeed;

#define PARTICLE_TYPE_LAUNCHER 0.0f
#define PARTICLE_TYPE_SHELL 1.0f
#define PARTICLE_TYPE_SECONDARY_SHELL 2.0f

vec3 GetRandomDir(float TexCoord)
{
    vec3 Dir = texture(gRandomTexture, TexCoord).xyz;
    Dir -= vec3(0.5, 0.5, 0.5);
    return Dir;
}

#define dist 0.2

void main()
{
    float Age = Age0[0] + gDeltaTimeMillis;
    
    if (Type0[0] == PARTICLE_TYPE_LAUNCHER) {
        if (Age >= gLauncherLifetime /*- (gLauncherLifetime - gDeltaTimeMillis * gPlayerSpeed / 1000)*/) {
            Type1 = PARTICLE_TYPE_SHELL;
            Age1 = 0.0;
            
            // Used to spawn particles around the emitters
            vec3 randomVec = GetRandomDir(gTime / 1000.0);
            randomVec.z = 0.0;
            randomVec.x = randomVec.x / 20;
            randomVec.y = randomVec.y / 15;
            Position1 = Position0[0] + randomVec;
            vec3 Dir = GetRandomDir(gTime/1000.0);
            Dir.y = max(Dir.y, 0.5);
            Dir.x = min(Dir.x, -0.3);
            Dir.z = min(Dir.z, -0.5);
            Velocity1 = normalize(Dir) / 5 * gPlayerSpeed / 1000;
            EmitVertex();
            
             randomVec = GetRandomDir(gTime / 1000.0);
            randomVec.z = 0.0;
            randomVec.x = randomVec.x / 20;
            randomVec.y = randomVec.y / 15;
            Position1 = Position0[0] + randomVec;
             Dir = GetRandomDir(gTime/1000.0);
            Dir.y = max(Dir.y, 0.5);
            Dir.x = min(Dir.x, -0.3);
            Dir.z = min(Dir.z, -0.5);
            Velocity1 = normalize(Dir) * gPlayerSpeed / 1000;
            
            EmitVertex();
            Age = 0.0;
        }
        
        Type1 = PARTICLE_TYPE_LAUNCHER;
        Position1 = Position0[0];
        Velocity1 = Velocity0[0];
        Age1 = Age;
        EmitVertex();
        EndPrimitive();
    }
    else {
        float DeltaTimeSecs = gDeltaTimeMillis / 1000.0f;
        float t1 = Age0[0] / 1000.0;
        float t2 = Age / 1000.0;
        vec3 DeltaP = DeltaTimeSecs * Velocity0[0];
        float dragComponent = -3 * gPlayerSpeed / 1000;
        vec3 DeltaV = vec3(DeltaTimeSecs) * vec3(0.0, 3.0, dragComponent);
        
        if (Type0[0] == PARTICLE_TYPE_SHELL)  {
            if (Age < gShellLifetime) {
                
                Type1 = PARTICLE_TYPE_SHELL;
                Position1 = Position0[0] + DeltaP;
                Velocity1 = Velocity0[0] + DeltaV;
                Age1 = Age;
                EmitVertex();
                EndPrimitive();
            }
            
        }
        
    }
}

