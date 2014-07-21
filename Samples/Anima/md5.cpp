#define _CRT_SECURE_NO_WARNINGS

#include "md5.h"

#include <stdio.h>
#include <string.h>
#include <ml.h>

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

    char *line = strtok((char*)inText->buffer, "\n");

    int int_val = 0;

    unsigned int mesh_index = 0;

    while (line)
    {
        if (sscanf( line, "MD5Version %d", &int_val ) == 1)
        {
            if (int_val != 10) return false;

            model->version = int_val;
        }

        else if (sscanf(line, "numJoints %d", &int_val) == 1)
        {
            model->numJoints = int_val;
            model->joints    = mem_raw_array<md5_joint_t>(outBinary, model->numJoints);
        }

        else if(sscanf(line, "numMeshes %d", &int_val) == 1)
        {
            model->numMeshes = int_val;
            model->meshes    = mem_raw_array<md5_mesh_t>(outBinary, model->numMeshes);
        }

        else if (!strncmp(line, "joints {", 8))
        {
            char name[64];
            unsigned int i      =  0;
            int          parent = -1;

            line = strtok(NULL, "\n");

            while (line[ 0 ] != '}')
            {
                ml::quat      rotation;
                ml::vec3      location;

                if (sscanf( line,
                    "%s %d ( %f %f %f ) ( %f %f %f )",
                    name, &parent,
                    &location.x, &location.y, &location.z,
                    &rotation.x, &rotation.y, &rotation.z) == 8 )
                {
                    build_w_quat(rotation);

                    md5_joint_t* joint = model->joints+i;

                    memcpy(joint->name, name, 64);
                    joint->parent   = parent;
                    joint->location = location;
                    joint->rotation = rotation;

                    ++i;
                }

                line = strtok(NULL, "\n");
            }
        }

        else if (!strncmp(line, "mesh {", 6))
        {
            md5_vertex_t    md5vertex;
            md5_weight_t    md5weight;
            uint16_t        md5triangle[3];
            char            shader[128];

            line = strtok(NULL, "\n");

            md5_mesh_t* mesh = model->meshes+mesh_index;

            while (line[ 0 ] != '}')
            {
                if (sscanf(line, " shader \"%[^\"]", shader) == 1)
                {
                    memcpy(mesh->shader, shader, 128);
                }

                else if (sscanf(line, " numverts %d", &int_val) == 1)
                {
                    mesh->numVertices = int_val;
                    mesh->vertices    = mem_raw_array<md5_vertex_t>(outBinary, mesh->numVertices);
                }

                else if (sscanf(line, " vert %d ( %f %f ) %d %d",
                                &int_val,
                                &md5vertex.u, &md5vertex.v,
                                &md5vertex.start,
                                &md5vertex.count) == 5)
                {
                    memcpy(&mesh->vertices[int_val], &md5vertex, sizeof(md5vertex));
                }

                else if (sscanf(line, " numtris %d", &int_val) == 1)
                {
                    mesh->numIndices = int_val * 3;
                    mesh->indices    = mem_raw_array<uint16_t>(outBinary, mesh->numIndices);
                }

                else if (sscanf(line,
                    " tri %d %hu %hu %hu",
                    &int_val,
                    &md5triangle[ 0 ],
                    &md5triangle[ 1 ],
                    &md5triangle[ 2 ]) == 4)
                {
                    memcpy(&mesh->indices[int_val*3], md5triangle, sizeof(md5triangle));
                }

                else if (sscanf(line, " numweights %d", &int_val) == 1)
                {
                    mesh->numWeights = int_val;
                    mesh->weights    = mem_raw_array<md5_weight_t>(outBinary, mesh->numWeights);
                }

                else if (sscanf(line,
                    " weight %d %d %f ( %f %f %f )",
                    &int_val,
                    &md5weight.joint,
                    &md5weight.bias,
                    &md5weight.location.x,
                    &md5weight.location.y,
                    &md5weight.location.z) == 6)

                {
                    memcpy(&mesh->weights[int_val], &md5weight, sizeof(md5weight));
                }

                line = strtok(NULL, "\n");
            }

            ++mesh_index;
        }

        line = strtok(NULL, "\n");
    }

    return true;
}

bool md5animConvertToBinary(memory_t* inText, memory_t* outBinary)
{
    md5_anim_t* anim = mem_raw_data<md5_anim_t>(outBinary);

    char *line = strtok((char*)inText->buffer, "\n");

    int int_val = 0;

    while (line)
    {
        if (sscanf(line, "MD5Version %d", &int_val) == 1)
        {
            if (int_val != 10) return false;

            anim->version = int_val;
        }

        else if (sscanf(line, "numFrames %d", &int_val) == 1)
        {
            anim->numFrames = int_val;
        }

        else if (sscanf(line, "numJoints %d", &int_val) == 1)
        {
            anim->numJoints = int_val;
            anim->frameData = mem_raw_array<md5_anim_data_t>(outBinary, 0); // get current pointer without incrementing allocated size
        }

        else if (sscanf(line, "frameRate %d", &int_val) == 1)
        { 
            anim->frameRate = int_val;
        }

        else if (sscanf(line, "frame %d", &int_val))
        {
            md5_anim_data_t* data = mem_raw_array<md5_anim_data_t>(outBinary, anim->numJoints);

            line = strtok(NULL, "\n");

            unsigned int i = 0;

            while (i != anim->numJoints)
            {
                ml::quat rotation;
                ml::vec3 location;

                if (sscanf( line,
                    " %f %f %f %f %f %f",
                    &location.x, &location.y, &location.z,
                    &rotation.x, &rotation.y, &rotation.z) == 6)
                {
                    build_w_quat(rotation);

                    data[i].location = location;
                    data[i].rotation = rotation;
                }

                line = strtok(NULL, "\n");

                ++i;
            }
        }

        line = strtok(NULL, "\n");	
    }

    return true;
}

