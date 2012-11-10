#include <ml.h>
#include <utils.h>

struct md5_joint_t;
struct md5_mesh_t;
struct md5_weight_t;
struct md5_vertex_t;
struct md5_anim_data_t;

struct md5_model_t
{
    int version;
    int numJoints;
    int numMeshes;

    md5_joint_t*    joints;
    md5_mesh_t*     meshes;
};

struct md5_joint_t
{
    char     name[64];
    int      parent;
    ml::vec3 location;
    ml::quat rotation;
};

struct md5_mesh_t
{
    char            shader[128];

    int             numVertices;
    int             numIndices;
    int             numWeights;

    md5_vertex_t*   vertices;
    md5_weight_t*   weights;
    uint16_t*       indices;
};

struct md5_weight_t
{
    int         joint;
    float       bias;
    ml::vec3    location;
};

struct md5_vertex_t
{
    float   u, v;
    int     start;
    int     count;
};

struct md5_anim_t
{
    int              version;
    int              numFrames;
    int              numJoints;
    int              frameRate;
    md5_anim_data_t* frameData;
};

struct md5_anim_data_t
{
    ml::quat rotation;
    ml::vec3 location;
};

bool md5meshConvertToBinary(memory_t* inText, memory_t* outBinary);
bool md5animConvertToBinary(memory_t* inText, memory_t* outBinary);

void md5BuildJoints(int            numJoints,
                    md5_joint_t*   joints,
                    int*           hierarchy,
                    ml::dual_quat* bindPose,
                    ml::dual_quat* invBindPose);

void md5BuildAnimations(int              numJoints,
                        int              numFrames,
                        int*             hierarchy,
                        md5_anim_data_t* animData,
                        ml::dual_quat*   framePoses);