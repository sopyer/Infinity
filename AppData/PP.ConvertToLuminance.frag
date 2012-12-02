in vec2 vUV;

layout(binding = 0) uniform sampler2D samImage;

const vec3 eyeSensitivity = vec3(0.3086, 0.6094, 0.0820);

void main()
{
    vec3  texel = texture2D(samImage, vUV).xyz;
    float grey  = dot(texel, eyeSensitivity);

    gl_FragColor =  vec4(grey, grey, grey, 1.0);
}