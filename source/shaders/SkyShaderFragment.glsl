#version 330 core

#define RAYLEIGH_CONSTANT -0.01
#define MIE_CONSTANT 0.63
#define SPOT_CONSTANT 0.99995

uniform mat4 uInvProjMatrix;
uniform mat4 uInvViewMatrix;
uniform vec3 uLightDir; // Direction /to/ the light
uniform vec4 uViewport;
uniform vec2 uFramebufferResolution;
uniform float uBrightness;

uniform float[64] uMusicSamples;

out vec4 fragColor;

const float intensity = 1.8;
const float surfaceHeight = 0.99;
const int stepCount = 4;
const float rayleighBrightness = 3.3;
const float mieBrightness = 0.1;
const float spotBrightness = 1000.0;
const float scatterStrength = 0.028;
const float rayleighStrength = 0.139;
//const float mieStrength = 0.0264;
const float rayleighCollectionPower = 0.81;
const float mieCollectionPower = 0.39;
const vec3 Kr = vec3(0.18867780436772762, 0.4978442963618773, 0.6616065586417131);

float rand(vec2 co)
{
   return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453);
}

// Removes the translation component of a matrix
mat4 removeTranslation(mat4 matrix) {
   matrix[3][0] = 0.0;
   matrix[3][1] = 0.0;
   matrix[3][2] = 0.0;

   return matrix;
}

// Gets the normal of a fragment in world space (ray into the sky)
vec3 getWorldNormal() {
   vec2 scale = uFramebufferResolution / uViewport.zw;
   vec2 fragCoord = (gl_FragCoord.xy - uViewport.xy) / uFramebufferResolution;
   fragCoord *= scale;
   fragCoord = (fragCoord - 0.5) * 2.0;

   vec4 deviceNormal = vec4(fragCoord, 0.0, 1.0);
   vec3 eyeNormal = normalize((uInvProjMatrix * deviceNormal).xyz);
   vec4 worldNormal = normalize(removeTranslation(uInvViewMatrix) * vec4(eyeNormal, 0.0));

   return worldNormal.xyz;
}

// Phase function - describes how much light is scattered toward the
// direction of the camera based on the angle between the light and camera
//
// alpha = cosine of the angle between the light and camera direction
// g = constant that affects the symmetry of the scattering; for Rayleigh,
//     g should be 0, and for Mie, g should be between -0.75 and -0.999
float phase(float alpha, float g) {
   float a = 3.0 * (1.0 - g * g);
   float b = 2.0 * (2.0 + g * g);
   float c = 1.0 + alpha * alpha;
   float d = pow(1.0 + g * g - 2.0 * g * alpha, 1.5);
   return (a / b) * (c / d);
}

// Determines the atmospheric depth for the given position and direction of
// the camera, by solving for the intersection point of the ray coming out of
// the camera and the 'edge' of the atmosphere (a sphere)
float atmosphericDepth(vec3 position, vec3 dir) {
   float a = dot(dir, dir);
   float b = 2.0 * dot(dir, position);
   float c = dot(position, position) - 1.0;
   float det = b * b - 4.0 * a * c;
   float detSqrt = sqrt(det);
   float q = (-b - detSqrt) / 2.0;
   float t1 = c / q;
   return t1;
}

// Calculates light cutoff on the horizon
float horizonExtinction(vec3 position, vec3 dir, float radius) {
   float u = dot(dir, -position);
   if (u < 0.0) {
      return 1.0;
   }

   vec3 near = position + u * dir;
   if (length(near) < radius) {
      return 0.0;
   }

   vec3 v2 = normalize(near) * radius - position;
   float diff = acos(dot(normalize(v2), dir));
   return smoothstep(0.0, 1.0, pow(diff * 2.0, 3.0));
}

vec3 absorb(float dist, vec3 color, float factor) {
   return color - color * pow(Kr, vec3(factor / dist));
}

void main() {
   vec3 eyeDir = getWorldNormal();
   float alpha = dot(eyeDir, uLightDir);
   //   float mieBrightness = uBrightness;
   float mieStrength = uBrightness;

   float rayleighFactor = phase(alpha, RAYLEIGH_CONSTANT) * rayleighBrightness;
   float mieFactor = phase(alpha, MIE_CONSTANT) * mieBrightness;
   float spot = smoothstep(0.0, 15.0, phase(alpha, SPOT_CONSTANT)) * spotBrightness;

   vec3 eyePos = vec3(0.0, surfaceHeight, 0.0);
   float eyeDepth = atmosphericDepth(eyePos, eyeDir);
   float stepLength = eyeDepth / float(stepCount);

   float eyeExtinction = horizonExtinction(eyePos, eyeDir, surfaceHeight - 0.15);

   vec3 rayleighCollected = vec3(0.0);
   vec3 mieCollected = vec3(0.0);

   for (int i = 0; i < stepCount; i++) {
      float sampleDistance = stepLength * float(i);
      vec3 position = eyePos + eyeDir * sampleDistance;
      float extinction = horizonExtinction(position, uLightDir, surfaceHeight - 0.35);
      float sampleDepth = atmosphericDepth(position, uLightDir);

      vec3 influx = absorb(sampleDepth, vec3(intensity), scatterStrength) * extinction;

      rayleighCollected += absorb(sampleDistance, Kr * influx, rayleighStrength);
      mieCollected += absorb(sampleDistance, influx, mieStrength);
   }

   rayleighCollected = (
      rayleighCollected *
      eyeExtinction *
      pow(eyeDepth, rayleighCollectionPower)
      ) / float(stepCount);
   mieCollected = (
      mieCollected *
      eyeExtinction *
      pow(eyeDepth, mieCollectionPower)
      ) / float(stepCount);

   fragColor = vec4(
      spot * mieCollected +
      mieFactor * mieCollected +
      rayleighFactor * rayleighCollected
      , 1.0);

   float size = 30.0;
   float prob = 0.95;

   vec2 fragCoord = (gl_FragCoord.xy - uViewport.xy) / uFramebufferResolution;
   vec2 pos = floor(1.0 / size * fragCoord.xy);

   float color = 0.0;
   float starValue = rand(pos);
   float xSample = 48 * fragCoord.x + 4;
   int left = int(floor(xSample));
   int right = left + 1;
   if (right >= 64) {
      right = 63;
   }
   
   float mSample = mix(uMusicSamples[right], uMusicSamples[left], xSample - left) * 5;
   if (starValue > prob)
   {
      vec2 center = size * pos + vec2(size, size) * 0.5;

      float t = 0.9 + 0.2 * sin(mSample + (starValue - prob) / (1.0 - prob) * 45.0);

      color = 1.0 - distance(fragCoord.xy, center) / (0.5 * size);
      color = color * t / (abs(fragCoord.y - center.y)) * t / (abs(fragCoord.x - center.x));
   }
   else if (rand(fragCoord.xy) > 0.99)
   {
      float r = rand(fragCoord.xy);
      color = mSample * r;
   }

   fragColor += vec4(vec3(color), 1.0);
}
