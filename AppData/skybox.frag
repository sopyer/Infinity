in vec3 vPos;

layout(location = 0, index = 0) out vec4 rt0;

layout(binding = 0) uniform samplerCube smpSkybox;

float saturate(float x)
{
	return clamp(x, 0, 1);
}

void main()
{
	//rt0 = vec4(normalize(vPos)*0.5 + 0.5, 1.0);
	//rt0 = texture(smpSkybox, vPos);
	vec4 px = texture(smpSkybox, vec3( 1,  0,  0));
	vec4 py = texture(smpSkybox, vec3( 0,  1,  0));
	vec4 pz = texture(smpSkybox, vec3( 0,  0,  1));
	vec4 nx = texture(smpSkybox, vec3(-1,  0,  0));
	vec4 ny = texture(smpSkybox, vec3( 0, -1,  0));
	vec4 nz = texture(smpSkybox, vec3( 0,  0, -1));
	
	rt0 = (saturate(vPos.x)*saturate(vPos.x)*px + saturate(-vPos.x)*saturate(-vPos.x)*abs(nx) + saturate(vPos.y)*saturate(vPos.y)*py + saturate(-vPos.y)*saturate(-vPos.y)*ny + saturate(vPos.z)*saturate(vPos.z)*pz + saturate(-vPos.z)*saturate(-vPos.z)*nz);
}