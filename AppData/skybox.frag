in vec3 vDir;

layout(location = 0, index = 0) out vec4 rt0;

layout(binding = 0) uniform samplerCube smpSkybox;

void main()
{
	rt0 = texture(smpSkybox, vDir);
}
