#version 330

in vec2 vUV;

layout(binding=0) uniform sampler2D samMeanPI;
layout(binding=1) uniform sampler2D samMeanPIII;

uniform float uEPS;

void main(void)
{
   vec4  meanPI   = texture2D(samMeanPI,   vUV);
   vec4  meanPIII = texture2D(samMeanPIII, vUV);
   
   vec3  meanP   = meanPI.rgb;
   vec3  meanPxI = meanPIII.rgb;
   float meanI   = meanPI.a;
   float meanIxI = meanPIII.a;
   
   float varI   = meanIxI - meanI * meanI;
   vec3  covIxP = meanPxI - meanI * meanP;
   
   vec3  a = covIxP / (varI + uEPS);
   vec3  b = meanP - a * meanI;
   
   gl_FragData[0] = vec4(a, 1.0);
   gl_FragData[1] = vec4(b, 1.0);
}
