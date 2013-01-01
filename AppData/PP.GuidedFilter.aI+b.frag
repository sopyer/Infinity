varying vec2 vUV;

layout(binding=0) uniform sampler2D samMeanA;
layout(binding=1) uniform sampler2D samMeanB;
layout(binding=2) uniform sampler2D samGuide;

void main(void)
{
   vec3  meanA   = texture2D(samMeanA, vUV).rgb;
   vec3  meanB   = texture2D(samMeanB, vUV).rgb;
   float I       = texture2D(samGuide, vUV).r;
   
   gl_FragColor = vec4(meanA * I + meanB, 1.0);
}