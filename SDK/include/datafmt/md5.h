#include <core/core.h>

struct md5_joint_t;
struct md5_mesh_t;
struct md5_weight_t;
struct md5_vertex_t;
struct md5_anim_data_t;

struct md5_model_t
{
    uint32_t  version;
    uint32_t  numJoints;
    uint32_t  numMeshes;

    md5_joint_t*  joints;
    md5_mesh_t*   meshes;
};

struct md5_joint_t
{
    char      name[64];
    int32_t   parent;
    ml::vec3  location;
    ml::quat  rotation;
};

struct md5_mesh_t
{
    char           shader[128];

    uint32_t       numVertices;
    uint32_t       numIndices;
    uint32_t       numWeights;

    md5_vertex_t*  vertices;
    md5_weight_t*  weights;
    uint16_t*      indices;
};

struct md5_weight_t
{
    int32_t   joint;
    float     bias;
    ml::vec3  location;
};

struct md5_vertex_t
{
    float     u, v;
    uint32_t  start;
    uint32_t  count;
};

struct md5_anim_t
{
    uint32_t          version;
    uint32_t          numFrames;
    uint32_t          numJoints;
    uint32_t          frameRate;
    md5_anim_data_t*  frameData;
};

struct md5_anim_data_t
{
    ml::quat  rotation;
    ml::vec3  location;
};

bool md5meshConvertToBinary(blob32_t inText, blob32_t outBinary);
bool md5animConvertToBinary(blob32_t inText, blob32_t outBinary);
