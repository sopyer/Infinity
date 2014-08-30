#version 440

layout(binding=0) uniform sampler2D samDiffuse;
layout(binding=1) uniform sampler2D samSpecular;
layout(binding=2) uniform sampler2D samNormal;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;

layout(std140, column_major) uniform;

layout(binding = 1) uniform Globals
{
    vec4  uMatDiffuse;
    vec3  uMatSpecular;
    vec3  uAmbientGlobal;
    float uR0;
    float uMatSpecPow;
};

#define DEBUG_SHADER

layout(binding = 2) buffer ClusterData
{
    uint uGridTileX;
    uint uGridTileY;
    uint uGridDimX;
    uint uGridDimY;
    float uInvZNear;
    float uLogScale;
#ifdef DEBUG_SHADER
    uint uDebugMaxClusters;
    uint uDebugMaxLightList;
    uint uDebugMaxLights;
    uint pad0, pad1, pad2;
#else
    uint pad0, pad1;
#endif
    struct 
    {
        uint offset;
        uint count;
    } uClusterLists[];
};

layout(binding = 3) buffer LightLists
{
    uint uLightList[];
};

layout(binding = 4) buffer Lights
{
    struct
    {
        vec3  pos;
        float range;
        vec3  color;
    } uLights[];
};

layout(location = 0, index = 0) out vec4 fragColor;

vec3 doLight(vec3 position, vec3 normal, vec3 diffuse, vec3 specular, float shininess, vec3 viewDir, vec3 lightPos, vec3 lightColor, float range)
{
    vec3 lightDir = lightPos - position;
    float dist = length(lightDir);
    float att = 1.0 - dist / range;

    if (att<=0.0) return vec3(0);

    lightDir = normalize(lightDir);
    float ndotL = clamp(dot(normal, lightDir), 0.0, 1.0);
    vec3 h = normalize(lightDir + viewDir);

    float normalizationFactor = ((shininess + 2.0) / 8.0);

    vec3 spec = specular * pow(max(0, dot(h, normal)), shininess) * normalizationFactor;

    return ndotL * att * lightColor * (diffuse + spec);
}

vec3 doLight(vec3 position, vec3 normal, vec3 diffuse, vec3 specular, float shininess, vec3 viewDir, uint lightIndex)
{
    vec3  lightPos   = uLights[lightIndex].pos;
    float lightRange = uLights[lightIndex].range;
    vec3  lightColor = uLights[lightIndex].color;

    return doLight(position, normal, diffuse, specular, shininess, viewDir, lightPos, lightColor, lightRange);
}

uvec3 calcClusterLoc(vec2 fragPos, float viewSpaceZ)
{
    // i and j coordinates are just the same as tiled shading, and based on screen space position.
    uvec2 l = ivec2(uint(fragPos.x) / uGridTileX, uint(fragPos.y) / uGridTileY);

    // k is based on the log of the view space Z coordinate.
    uint gridLocZ = uint(log(-viewSpaceZ * uInvZNear) * uLogScale);

    return uvec3(l, gridLocZ);
}


uint calcClusterOffset(uvec3 clusterLoc)
{
    return (clusterLoc.z * uGridDimY + clusterLoc.y) * uGridDimX + clusterLoc.x;
}

uint calcClusterOffset(vec2 fragPos, float viewSpaceZ)
{
    return calcClusterOffset(calcClusterLoc(fragPos, viewSpaceZ));
}

vec3 evalClusteredShading(in vec3 diffuse, in vec3 specular, in float shininess, in vec3 position, in vec3 normal, in vec3 viewDir)
{
    // fetch cluster data (i.e. offset to light indices, and numer of lights) from grid buffer.
    uint  idx = calcClusterOffset(gl_FragCoord.xy, position.z);

    uint lightOffset = uClusterLists[idx].offset;
    uint lightCount  = uClusterLists[idx].count;

    vec3 shading = vec3(0.0, 0.0, 0.0);
#ifdef DEBUG_SHADER
    if (idx>=uDebugMaxClusters) return vec3(1, 0, 0);
    if (lightOffset+lightCount>uDebugMaxLightList) return vec3(0, 1, 0);
#endif

    for (uint i = 0; i < lightCount; ++i)
    {
        // fetch light index from list of lights for the cluster.
        uint lightIndex = uLightList[lightOffset + i];
#ifdef DEBUG_SHADER
        if (lightIndex>=uDebugMaxLights) return vec3(0, 0, 1);
#endif
        // compute and accumulate shading.
        shading += doLight(position, normal, diffuse, specular, shininess, viewDir, lightIndex);
    }

    return shading;
}

void main()
{
    vec3 position = vPosition;
    vec3 viewDir = normalize(-vPosition);

    vec3 normal   = vNormal;
    vec3 diffuse  = texture2D(samDiffuse,  vTexCoord0).rgb * uMatDiffuse.rgb;
    vec3 specular = texture2D(samSpecular, vTexCoord0).rgb * uMatSpecular;
    vec3 ambient  = diffuse * uAmbientGlobal;

    float fresnelTerm = pow(clamp(1.0 + dot(-viewDir, normal), 0.0, 1.0), 5.0);
    vec3  fresnelSpec = specular * (uR0 + (vec3(1.0) - uR0) * fresnelTerm);

    vec3 lighting = evalClusteredShading(diffuse, fresnelSpec, uMatSpecPow, position, normal, viewDir);

    fragColor = vec4(lighting + ambient, uMatDiffuse.a + (1.0 - uMatDiffuse.a) * fresnelTerm);

    //fragColor = texture2D(samDiffuse, vTexCoord0);
}
