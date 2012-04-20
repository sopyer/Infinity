layout(std140, column_major) uniform;

#define ATTR_POSITION	0
#define ATTR_PATCH_BASE	1
#define ATTR_LEVEL		2

layout(location = ATTR_POSITION)		in vec2		aVertex;

//TODO: check whether some scales can be merged?

//TODO: remove aLevel and replace it with aMorphParams and aPatchScale params
//      and temporary aColor - make it work on SM 3.0
#ifdef ENABLE_INSTANCING
layout(location = ATTR_PATCH_BASE)		in vec2		aPatchBase;
layout(location = ATTR_LEVEL)			in vec2		aLevel;
#else
uniform uniPatch
{
	vec2 aPatchBase;
	vec2 aLevel;  //Just in case, AMD seems to use actual size and does not pad uniform blocks :(
};
#endif

#define MAX_LOD_COUNT 8

uniform uniTerrain
{
	vec4		uHMDim;
	vec3		uOffset;
	vec3		uScale;

	// Morph parameter are evaluated as follows:
	// uMorphParam[...].x = 1.0/(morphEnd-morphStart)
	// uMorphParam[...].y = -morphStart/(morphEnd-morphStart)
	vec2		uMorphParams[MAX_LOD_COUNT];
	vec3		uColors[MAX_LOD_COUNT];
};

uniform uniView
{
	mat4		uMVP;
	vec4		uLODViewK;
};

uniform sampler2D	uHeightmap;

out float vHeight;
out vec4  vColor;
out vec2 vUV;

void main()
{
	vec2  patchScale = vec2(1<<int(aLevel.x));
	vec2  morphDir   = fract(aVertex*0.5)*2.0*patchScale;

	vec3  vertexPos;
	float distance, morphK;
	
	vertexPos.xz = aPatchBase + patchScale*aVertex;
	vertexPos.xz = clamp(vertexPos.xz, vec2(0.0), uHMDim.xy-vec2(1.0)); //TODO: optimize!!!
	vertexPos.y  = 0.0;

	//Applying morphing for seamless connectivity

	distance     = length(vertexPos*uScale + uLODViewK.xyz);
	morphK       = clamp(distance*uMorphParams[int(aLevel.x)].x + uMorphParams[int(aLevel.x)].y, 0.0, 1.0);
	vertexPos.xz = vertexPos.xz+morphDir*morphK;
	vec2 uv      = (vertexPos.xz+vec2(0.5))*uHMDim.zw; //TODO: optimize!!!
	vertexPos.y  = textureLod(uHeightmap, uv, 0).r;

	//Varyings

	gl_Position = uMVP*vec4(vertexPos*uScale+uOffset, 1); //TODO: optimize!!!
	vHeight     = vertexPos.y;
	vColor      = vec4(uColors[int(aLevel.x)], 1);
	vUV         = vertexPos.xz*uHMDim.zw;
	vec4 p = uMVP*vec4(vertexPos*uScale+uOffset, 1); //TODO: optimize!!!
	p.z = (p.z>0 && p.z>p.w)?p.w:p.z;
	gl_Position = p;
}

