#pragma once

#include <vg/openvg.h>
#include <core/ml.h>

namespace vg
{
    struct B3Vertex
    {
        ml::vec2  p;
        ml::vec3  klm;
    };

    struct geometry_t
    {
        size_t  numIndices;
        size_t  numVertices;
        size_t  numB3Vertices;
        
        uint16_t*  indices;
        ml::vec2*  vertices;
        B3Vertex*  b3vertices;
    };

    struct path_data_t
    {
        float xmin, ymin, xmax, ymax;

        size_t  numVertices;
        size_t  numIndices;
        size_t  numB3Vertices;
        size_t  numB3Indices;

        ml::vec2*  vertices;
        uint16_t*  indices;

        B3Vertex*  b3vertices;
        uint16_t*  b3indices;
    };

    uint16_t     geomAddVertex    (geometry_t* geom, const ml::vec2& v);
    void         geomAddTri       (geometry_t* geom, uint16_t i0, uint16_t i1, uint16_t i2);
    void         geomAddB3Vertices(geometry_t* geom, ml::vec2 pos[4], ml::vec3 klm[4]);
    path_data_t* geomToPath       (geometry_t* geom);

    void rasterizeEvenOddAA(path_data_t* path);

    void stencilPath(path_data_t* path, int useAA, int useNonZero);
}
