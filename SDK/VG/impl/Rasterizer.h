#pragma once

#include <stdint.h>
#include <glm/glm.h>
#include <vg/openvg.h>
#include <opengl.h>
#include <cassert>

#include "DataTypes.h"

namespace impl
{
    struct Bezier3Vertex
    {
        float x, y;
        float k, l, m;

        static void construct(Bezier3Vertex* v, const glm::vec2& pos, const glm::vec3& klm)
        {
            v->x = pos.x; v->y = pos.y;
            v->k = klm.x; v->l= klm.y; v->m = klm.z;
        }
    };

    struct Geometry
    {
        float xmin, ymin, xmax, ymax;

        Array<glm::vec2> shapeVertices;
        Array<uint16_t>  shapeIndices;

        uint16_t shapeAddVertex(const glm::vec2& v)
        {
            size_t idx = shapeVertices.size();
            shapeVertices.pushBack(v);
            assert(idx<=USHRT_MAX);
            return (uint16_t)idx;
        }

        void shapeAddTri(uint16_t i0, uint16_t i1, uint16_t i2)
        {
            shapeIndices.push_back(i0);
            shapeIndices.push_back(i1);
            shapeIndices.push_back(i2);
        }

        Array<Bezier3Vertex> bezier3Vertices;
        Array<uint16_t>      bezier3Indices;

        void bezier3AddVertices(glm::vec2 pos[4], glm::vec3 klm[4])
        {
            Bezier3Vertex* v = bezier3Vertices.expand(4);

            Bezier3Vertex::construct(v++, pos[0], klm[0]);
            Bezier3Vertex::construct(v++, pos[1], klm[1]);
            Bezier3Vertex::construct(v++, pos[2], klm[2]);
            Bezier3Vertex::construct(v++, pos[3], klm[3]);
        }

        //TODO: change this properly later
        void bezier3GenIndices()
        {
            assert(bezier3Vertices.size()%4 == 0);
            assert(bezier3Vertices.size()<=USHRT_MAX);

            for (size_t i = 0; i<bezier3Vertices.size(); i+=4)
            {
                bezier3Indices.push_back((uint16_t)i+0);
                bezier3Indices.push_back((uint16_t)i+1);
                bezier3Indices.push_back((uint16_t)i+3);

                bezier3Indices.push_back((uint16_t)i+1);
                bezier3Indices.push_back((uint16_t)i+2);
                bezier3Indices.push_back((uint16_t)i+3);
            }
        }
    };

    void rasterizeEvenOddA2C(Geometry& geom);
    void rasterizeNonZeroA2C(Geometry& geom);
    void rasterizeEvenOdd(Geometry& geom);
    void rasterizeEvenOddAA(Geometry& geom);
    void rasterizeNonZero(Geometry& geom);

    void stencilPath(Geometry* geom, int useAA, int useNonZero);
}
