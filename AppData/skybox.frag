in vec3 vPos;

layout(location = 0, index = 0) out vec4 rt0;

void main()
{
	rt0 = vec4(normalize(vPos)*0.5 + 0.5, 1.0);
}