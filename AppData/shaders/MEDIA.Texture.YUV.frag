
layout(binding = 0) uniform sampler2D samY;
layout(binding = 1) uniform sampler2D samU;
layout(binding = 2) uniform sampler2D samV;

in vec2  faUV0;
in vec4  faColor;

layout(location = 0, index = 0) out vec4  rt0;

void main()
{
    float y   = texture2D(samY, faUV0).r;
    float u   = texture2D(samU, faUV0).r - 0.5;
    float v   = texture2D(samV, faUV0).r - 0.5;

    vec3 rgb = vec3(y) + u*vec3(0.0, -0.344, 1.770) + v*vec3(1.403, -0.714, 0.0);

    rt0 = faColor * vec4(rgb, 1.0);
}
