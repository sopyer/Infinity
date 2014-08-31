#version 440

//#define DEBUG_SHADER

//!!!!!TODO: use ARB_robustness and SSBO when driver will be fixed

layout(std140, column_major) uniform;

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoord0;

layout(binding=0) uniform sampler2D samDiffuse;
layout(binding=1) uniform sampler2D samSpecular;
layout(binding=2) uniform sampler2D samNormal;

layout(binding=3) uniform isamplerBuffer samClusterData;
layout(binding=4) uniform isamplerBuffer samLightListData;
layout(binding=5) uniform samplerBuffer  samLightData;

layout(binding = 1) uniform MaterialData
{
    vec4  uMatDiffuse;
    vec3  uMatSpecular;
    vec3  uAmbientGlobal;
    float uR0;
    float uMatSpecPow;
};

layout(binding = 2) uniform ClusterData
{
    int uGridTileX;
    int uGridTileY;
    int uGridDimX;
    int uGridDimY;
    float uInvZNear;
    float uLogScale;
#ifdef DEBUG_SHADER
    int uDebugMaxClusters;
    int uDebugMaxLightList;
    int uDebugMaxLights;
#endif
};

layout(location = 0, index = 0) out vec4 fragColor;

vec3 doLight(vec3 position, vec3 normal, vec3 diffuse, vec3 specular, float shininess, vec3 viewDir, vec3 lightPos, vec3 lightColor, float range)
{
    vec3 lightDir = lightPos - position;

    float dist = length(lightDir);

    if (dist>range) return vec3(0);

    lightDir = normalize(lightDir);

    float att     = 1.0 - dist / range;
    vec3  halfVec = normalize(lightDir + viewDir);
    float NdotL   = clamp(dot(normal, lightDir), 0.0, 1.0);
    float NdotH   = clamp(dot(halfVec, normal),  0.0, 1.0);

    float normFactor = shininess*0.125+0.25;//((shininess + 2.0) / 8.0);

    vec3 spec = (normFactor * pow(NdotH, shininess)) * specular;
    vec3 diff = NdotL * diffuse;

    return att * lightColor * (diff + spec);
}

int calcClusterIndex(vec2 fragPos, float viewSpaceZ)
{
    int clusterLocX = int(fragPos.x) / uGridTileX;
    int clusterLocY = int(fragPos.y) / uGridTileY;
    int clusterLocZ = int(log(-viewSpaceZ * uInvZNear) * uLogScale);

    return (clusterLocZ * uGridDimY + clusterLocY) * uGridDimX + clusterLocX;
}

vec3 evalClusteredShading(in vec3 diffuse, in vec3 specular, in float shininess, in vec3 position, in vec3 normal, in vec3 viewDir)
{
    int  idx = calcClusterIndex(gl_FragCoord.xy, position.z);

    ivec2 clusterData = texelFetch(samClusterData, idx).rg;
    int lightOffset = clusterData.r;
    int lightCount  = clusterData.g;

    vec3 shading = vec3(0.0, 0.0, 0.0);
#ifdef DEBUG_SHADER
    if (idx>=uDebugMaxClusters) return vec3(1, 0, 0);
    if (lightOffset+lightCount>uDebugMaxLightList) return vec3(0, 1, 0);
#endif

    for (int i = 0; i < lightCount; ++i)
    {
        int lightIndex =  texelFetch(samLightListData, lightOffset + i).r;
#ifdef DEBUG_SHADER
        if (lightIndex>=uDebugMaxLights) return vec3(0, 0, 1);
#endif

        vec4 lightPosRange = texelFetch(samLightData, lightIndex*2+0);
        vec4 lightColor    = texelFetch(samLightData, lightIndex*2+1);

        shading += doLight(position, normal, diffuse, specular, shininess, viewDir, lightPosRange.xyz, lightColor.xyz, lightPosRange.w);
    }

    return shading;
}

void main()
{
    vec3 diffuse  = texture2D(samDiffuse,  vTexCoord0).rgb * uMatDiffuse.rgb;
    vec3 specular = texture2D(samSpecular, vTexCoord0).rgb * uMatSpecular;

    vec3 position = vPosition;
    vec3 normal   = vNormal;
    vec3 viewDir = normalize(-vPosition);

    float fresnelTerm = pow(clamp(1.0 + dot(-viewDir, normal), 0.0, 1.0), 5.0);
    vec3  fresnelSpec = specular * (uR0 + (1.0 - uR0) * fresnelTerm);

    vec3 direct   = evalClusteredShading(diffuse, fresnelSpec, uMatSpecPow, position, normal, viewDir);
    vec3 indirect = diffuse * uAmbientGlobal;

    fragColor = vec4(direct + indirect, uMatDiffuse.a + (1.0 - uMatDiffuse.a) * fresnelTerm);
}
