in vec3 vPos;

layout(location = 0, index = 0) out vec4 rt0;

layout(binding = 0) uniform samplerCube smpSkybox;

void main()
{
	//rt0 = vec4(normalize(vPos)*0.5 + 0.5, 1.0);
	rt0 = texture(smpSkybox, vPos);
}