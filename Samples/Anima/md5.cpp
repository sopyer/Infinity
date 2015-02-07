#include "md5.h"

const char endl[] = "\r\n";

void build_w_quat( ml::quat& quat )
{
    float t = 1.0f - ( quat.x * quat.x ) - ( quat.y * quat.y ) - ( quat.z * quat.z );
    if ( t < 0.0f )
    {
        quat.w = 0.0f;
    }
    else
    {
        quat.w = -ml::sqrt(t);
    }
}

bool md5meshConvertToBinary(memory_t* inText, memory_t* outBinary)
{
    md5_model_t* model = mem_raw_data<md5_model_t>(outBinary);

    mem_set(outBinary->buffer, outBinary->size, 0);

    const char* str = (char*)inText->buffer;
    rsize_t     str_size = inText->size;
    const char* line = NULL;
    rsize_t     line_size = 0;
    cstr_tokenize(endl, &str, &str_size, &line, &line_size);

    int int_val = 0;
    int ind;

    unsigned int mesh_index = 0;

    while (line)
    {
        if (cstr_scanf(line, line_size, "MD5Version %d", &int_val) == 1)
        {
            if (int_val != 10) return false;

            model->version = int_val;
        }

        else if (cstr_scanf(line, line_size, "numJoints %d", &int_val) == 1)
        {
            model->numJoints = int_val;
            model->joints    = mem_raw_array<md5_joint_t>(outBinary, model->numJoints);
        }

        else if(cstr_scanf(line, line_size, "numMeshes %d", &int_val) == 1)
        {
            model->numMeshes = int_val;
            model->meshes    = mem_raw_array<md5_mesh_t>(outBinary, model->numMeshes);
        }

        else if (cstr_compare(line, 8, "joints {", &ind) == EOK && ind == 0)
        {
            char name[64];
            unsigned int i      =  0;
            int          parent = -1;

            cstr_tokenize(endl, &str, &str_size, &line, &line_size);

            while (line_size && line[ 0 ] != '}')
            {
                ml::quat      rotation;
                ml::vec3      location;

                if (cstr_scanf(line, line_size,
                    "%s %d ( %f %f %f ) ( %f %f %f )",
                    name, ARRAY_SIZE(name), &parent,
                    &location.x, &location.y, &location.z,
                    &rotation.x, &rotation.y, &rotation.z) == 8)
                {
                    build_w_quat(rotation);

                    md5_joint_t* joint = model->joints+i;

                    mem_copy(joint->name, name, 64);
                    joint->parent   = parent;
                    joint->location = location;
                    joint->rotation = rotation;

                    ++i;
                }

                cstr_tokenize(endl, &str, &str_size, &line, &line_size);
            }
        }

        else if (cstr_compare(line, 6, "mesh {", &ind) == EOK && ind == 0)
        {
            md5_vertex_t    md5vertex;
            md5_weight_t    md5weight;
            uint16_t        md5triangle[3];
            char            shader[128];

            cstr_tokenize(endl, &str, &str_size, &line, &line_size);

            md5_mesh_t* mesh = model->meshes+mesh_index;

            while (line_size && line[ 0 ] != '}')
            {
                if (cstr_scanf(line, line_size, " shader \"%[^\"]", shader, ARRAY_SIZE(shader)) == 1)
                {
                    mem_copy(mesh->shader, shader, 128);
                }

                else if (cstr_scanf(line, line_size, " numverts %d", &int_val) == 1)
                {
                    mesh->numVertices = int_val;
                    mesh->vertices    = mem_raw_array<md5_vertex_t>(outBinary, mesh->numVertices);
                }

                else if (cstr_scanf(line, line_size, " vert %d ( %f %f ) %d %d",
                                &int_val,
                                &md5vertex.u, &md5vertex.v,
                                &md5vertex.start,
                                &md5vertex.count) == 5)
                {
                    mem_copy(&mesh->vertices[int_val], &md5vertex, sizeof(md5vertex));
                }

                else if (cstr_scanf(line, line_size, " numtris %d", &int_val) == 1)
                {
                    mesh->numIndices = int_val * 3;
                    mesh->indices    = mem_raw_array<uint16_t>(outBinary, mesh->numIndices);
                }

                else if (cstr_scanf(line, line_size, " tri %d %hu %hu %hu",
                    &int_val,
                    &md5triangle[ 0 ],
                    &md5triangle[ 1 ],
                    &md5triangle[ 2 ]) == 4)
                {
                    mem_copy(&mesh->indices[int_val*3], md5triangle, sizeof(md5triangle));
                }

                else if (cstr_scanf(line, line_size, " numweights %d", &int_val) == 1)
                {
                    mesh->numWeights = int_val;
                    mesh->weights    = mem_raw_array<md5_weight_t>(outBinary, mesh->numWeights);
                }

                else if (cstr_scanf(line, line_size, " weight %d %d %f ( %f %f %f )",
                    &int_val,
                    &md5weight.joint,
                    &md5weight.bias,
                    &md5weight.location.x,
                    &md5weight.location.y,
                    &md5weight.location.z) == 6)

                {
                    mem_copy(&mesh->weights[int_val], &md5weight, sizeof(md5weight));
                }

                cstr_tokenize(endl, &str, &str_size, &line, &line_size);
            }

            ++mesh_index;
        }

        cstr_tokenize(endl, &str, &str_size, &line, &line_size);
    }

    return true;
}

bool md5animConvertToBinary(memory_t* inText, memory_t* outBinary)
{
    md5_anim_t* anim = mem_raw_data<md5_anim_t>(outBinary);

    const char* str = (char*)inText->buffer;
    rsize_t     str_size = inText->size;
    const char* line = NULL;
    rsize_t     line_size = 0;
    cstr_tokenize(endl, &str, &str_size, &line, &line_size);

    int int_val = 0;

    while (line)
    {
        if (cstr_scanf(line, line_size, "MD5Version %d", &int_val) == 1)
        {
            if (int_val != 10) return false;

            anim->version = int_val;
        }

        else if (cstr_scanf(line, line_size, "numFrames %d", &int_val) == 1)
        {
            anim->numFrames = int_val;
        }

        else if (cstr_scanf(line, line_size, "numJoints %d", &int_val) == 1)
        {
            anim->numJoints = int_val;
            anim->frameData = mem_raw_array<md5_anim_data_t>(outBinary, 0); // get current pointer without incrementing allocated size
        }

        else if (cstr_scanf(line, line_size, "frameRate %d", &int_val) == 1)
        { 
            anim->frameRate = int_val;
        }

        else if (cstr_scanf(line, line_size, "frame %d", &int_val))
        {
            md5_anim_data_t* data = mem_raw_array<md5_anim_data_t>(outBinary, anim->numJoints);

            cstr_tokenize(endl, &str, &str_size, &line, &line_size);

            unsigned int i = 0;

            while (i != anim->numJoints)
            {
                ml::quat rotation;
                ml::vec3 location;

                if (cstr_scanf(line, line_size,
                    " %f %f %f %f %f %f",
                    &location.x, &location.y, &location.z,
                    &rotation.x, &rotation.y, &rotation.z) == 6)
                {
                    build_w_quat(rotation);

                    data[i].location = location;
                    data[i].rotation = rotation;
                }

                cstr_tokenize(endl, &str, &str_size, &line, &line_size);

                ++i;
            }
        }

        cstr_tokenize(endl, &str, &str_size, &line, &line_size);
    }

    return true;
}

