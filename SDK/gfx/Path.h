#pragma once

#include <vg/openvg.h>
#include <gfx/gfx.h>

namespace vg
{
    struct geometry_t
    {
        uint16_t*           indices;
        vf::p2_vertex_t*    vertices;
        vf::p2uv3_vertex_t* b3vertices;

        uint32_t  numIndices;
        uint32_t  numVertices;
        uint32_t  numB3Vertices;
    };

    struct path_data_t
    {
        float xmin, ymin, xmax, ymax;

        uint16_t  gpuMemHandle;

        uint32_t  baseVertex;
        uint32_t  offsetIndices;
        uint32_t  numIndices;

        uint32_t  baseB3Vertex;
        uint32_t  offsetB3Indices;
        uint32_t  numB3Indices;
    };

    uint16_t     geomAddVertex    (geometry_t* geom, const ml::vec2& v);
    void         geomAddTri       (geometry_t* geom, uint16_t i0, uint16_t i1, uint16_t i2);
    void         geomAddB3Vertices(geometry_t* geom, ml::vec2 pos[4], ml::vec3 klm[4]);
    path_data_t* geomToPath       (geometry_t* geom);

    void stencilPath(path_data_t* path, int useAA);
}
