#include <gfx/gfx.h>
#include "gfx_res.h"
#include "Path.h"

namespace vg
{
    uint16_t geomAddVertex(geometry_t* geom, const ml::vec2& v)
    {
        size_t idx = geom->numVertices;

        assert(idx<USHRT_MAX);
        geom->vertices[geom->numVertices].x = v.x;
        geom->vertices[geom->numVertices].y = v.y;

        ++geom->numVertices;

        return (uint16_t)idx;
    }

    void geomAddTri(geometry_t* geom, uint16_t i0, uint16_t i1, uint16_t i2)
    {
        geom->indices[geom->numIndices++] = i0;
        geom->indices[geom->numIndices++] = i1;
        geom->indices[geom->numIndices++] = i2;
    }

    void geomAddB3Vertices(geometry_t* geom, ml::vec2 pos[4], ml::vec3 klm[4])
    {
        size_t idx = geom->numB3Vertices;
        assert(idx+4<USHRT_MAX);

        vf::set(&geom->b3vertices[idx++], pos[0].x, pos[0].y, klm[0].x, klm[0].y, klm[0].z);
        vf::set(&geom->b3vertices[idx++], pos[1].x, pos[1].y, klm[1].x, klm[1].y, klm[1].z);
        vf::set(&geom->b3vertices[idx++], pos[2].x, pos[2].y, klm[2].x, klm[2].y, klm[2].z);
        vf::set(&geom->b3vertices[idx++], pos[3].x, pos[3].y, klm[3].x, klm[3].y, klm[3].z);

        geom->numB3Vertices = idx;
    }

    path_data_t* geomToPath(geometry_t* geom)
    {
        assert(geom->numB3Vertices % 4 == 0);
        assert(geom->numB3Vertices <= USHRT_MAX);

        path_data_t* path = mem::alloc<path_data_t>(gfx::memArena);

        memset(path, 0, sizeof(path_data_t));

        //stupid initialization
        if (geom->numVertices > 0)
        {
            path->xmin = path->xmax = geom->vertices[0].x;
            path->ymin = path->ymax = geom->vertices[0].y;
        }
        else if (geom->numB3Vertices > 0)
        {
            path->xmin = path->xmax = geom->b3vertices[0].x;
            path->ymin = path->ymax = geom->b3vertices[0].y;
        }
        else
        {
            path->xmin = path->ymin = path->xmax = path->ymax = 0;
            return path;
        }

        for(size_t i = 0; i < geom->numVertices; ++i)
        {
            path->xmin = core::min(path->xmin, geom->vertices[i].x);
            path->ymin = core::min(path->ymin, geom->vertices[i].y);
            path->xmax = core::max(path->xmax, geom->vertices[i].x);
            path->ymax = core::max(path->ymax, geom->vertices[i].y);
        }

        for(size_t i = 0; i < geom->numB3Vertices; ++i)
        {
            path->xmin = core::min(path->xmin, geom->b3vertices[i].x);
            path->ymin = core::min(path->ymin, geom->b3vertices[i].y);
            path->xmax = core::max(path->xmax, geom->b3vertices[i].x);
            path->ymax = core::max(path->ymax, geom->b3vertices[i].y);
        }


        uint32_t  verticesSize = sizeof(vf::p2_vertex_t) * geom->numVertices;
        uint32_t  indicesSize  = sizeof(uint16_t) * geom->numIndices;
        uint32_t  numB3Indices = geom->numB3Vertices / 4 * 6;
        uint32_t  b3verticesSize = sizeof(vf::p2uv3_vertex_t) * geom->numB3Vertices;
        uint32_t  b3indicesSize  = sizeof(uint16_t) * numB3Indices;
        uint32_t  verticesBase, b3verticesBase;
        uint32_t  totalSize = 0;
        
        totalSize += indicesSize + b3indicesSize;
        totalSize += verticesSize + sizeof(vf::p2_vertex_t);
        totalSize += b3verticesSize + sizeof(vf::p2uv3_vertex_t);

        path->gpuMemHandle = etlsf_alloc_range(gfx_res::vgGArena, totalSize);
        uint32_t  baseOffset = etlsf_alloc_offset(gfx_res::vgGArena, path->gpuMemHandle);

        uint8_t*  basePtr    = (uint8_t*)glMapNamedBufferRange(gfx_res::buffer, baseOffset, totalSize, GL_MAP_WRITE_BIT);

        assert ((baseOffset&0x01) == 0);

        verticesBase   = core::align_up<vf::p2_vertex_t>   (baseOffset + indicesSize + b3indicesSize);
        b3verticesBase = core::align_up<vf::p2uv3_vertex_t>(verticesBase + verticesSize);

        if (geom->numIndices)
        {
            vf::p2_vertex_t* vertices = (vf::p2_vertex_t*)(basePtr + (verticesBase - baseOffset));
            memcpy(vertices, geom->vertices, verticesSize);

            uint16_t* indices = (uint16_t*)basePtr;
            memcpy(indices, geom->indices, indicesSize);

            path->baseVertex    = verticesBase / sizeof(vf::p2_vertex_t);
            path->offsetIndices = baseOffset;
            path->numIndices    = geom->numIndices;
        }

        if (geom->numB3Vertices)
        {
            vf::p2uv3_vertex_t* b3vertices = (vf::p2uv3_vertex_t*)(basePtr + (b3verticesBase - baseOffset));
            memcpy(b3vertices, geom->b3vertices, b3verticesSize);

            uint16_t* b3indices = (uint16_t*)(basePtr + indicesSize);
            numB3Indices = 0;
            for (size_t i = 0; i<geom->numB3Vertices; i+=4)
            {
                b3indices[numB3Indices++] = (uint16_t)i+0;
                b3indices[numB3Indices++] = (uint16_t)i+1;
                b3indices[numB3Indices++] = (uint16_t)i+3;

                b3indices[numB3Indices++] = (uint16_t)i+1;
                b3indices[numB3Indices++] = (uint16_t)i+2;
                b3indices[numB3Indices++] = (uint16_t)i+3;
            }

            path->baseB3Vertex    = b3verticesBase / sizeof(vf::p2uv3_vertex_t);
            path->offsetB3Indices = baseOffset + indicesSize;
            path->numB3Indices    = numB3Indices;
        }

        glUnmapNamedBuffer(gfx_res::buffer);

        return path;
    }

    void subdivide(v128 cubic[4], float t, v128 subCubic1[4], v128 subCubic2[4])
    {
        v128 p0 = cubic[0];
        v128 p1 = cubic[1];
        v128 p2 = cubic[2];
        v128 p3 = cubic[3];

        v128 p01 = vi_lerp(p0, p1, t);
        v128 p12 = vi_lerp(p1, p2, t);
        v128 p23 = vi_lerp(p2, p3, t);

        v128 p012 = vi_lerp(p01, p12, t);
        v128 p123 = vi_lerp(p12, p23, t);

        v128 p0123 = vi_lerp(p012, p123, t);

        subCubic1[0] = p0;
        subCubic1[1] = p01;
        subCubic1[2] = p012;
        subCubic1[3] = p0123;

        subCubic2[0] = p0123;
        subCubic2[1] = p123;
        subCubic2[2] = p23;
        subCubic2[3] = p3;
    }

    void solveQuadratic(float k[3], float r[4])
    {
        float a = k[0], b = k[1], c = k[2];
        float D = b*b - 4*a*c;

        assert(D >= 0.0f);

        if (D == 0.0f)
        {
            r[0] = r[2] = -b;
            r[1] = r[3] = 2*a;
        }
        else
        {
            float rr = -b + (b < 0 ? 1 : -1)*ml::sqrt(D);

            r[0] = rr;  r[1] = 2*a;
            r[2] = 2*c; r[3] = rr;
        }
    }

    void normSquare(float& x, float& y)
    {
        float s;

        s = core::max(ml::abs(x), ml::abs(y));
        s = s > ML_C_EPS7 ? s : 1.0f;

        x /= s; y /= s;
    }

    void bezier3MakeImplicit(v128 pos[4], v128 klm[4], int& subdPtCount, float* subdPts)
    {
        //Bezier basis
        //bb[3] = { pos[1]-pos[0], pos[2]-pos[0], pos[3]-pos[0] };
        v128 bb0 = vi_sub(pos[1], pos[0]);
        v128 bb1 = vi_sub(pos[2], pos[0]);
        v128 bb2 = vi_sub(pos[3], pos[0]);

        //Transform from Bezier to power basis
        //pb[3] = { 3.0f*bb[0], -6.0f*bb[0] +  3.0f*bb[1], 3.0f*bb[0] + -3.0f*bb[1] + bb[2] };
        v128 pb0 = vi_mul(vi_set_all( 3.0f), bb0);
        v128 pb1 = vi_mad(vi_set_all(-6.0f), bb0, vi_mul(vi_set_all( 3.0f), bb1));
        v128 pb2 = vi_mad(vi_set_all( 3.0f), bb0, vi_mad(vi_set_all(-3.0f), bb1, bb2));

        // make xxx and yyy
        v128 xy[2];
        v128 m[4] = {pb0, pb1, pb2, vi_set_zero()};
        ml::transpose_mat4x2(xy, m);

        //d0 =  pb[1].x*pb[2].y - pb[1].y*pb[2].x;
        //d1 =  pb[2].x*pb[0].y - pb[2].y*pb[0].x;
        //d2 =  pb[0].x*pb[1].y - pb[0].y*pb[1].x;
        v128 vd = vi_cross3(xy[0], xy[1]);

        // Zero values less then epsilon in order 
        // to mitigates precision issues
        v128 mask = vi_cmp_gt(vi_abs(vd), vi_set_all(ML_C_EPS7));
        vd = vi_and(vd, mask);

        float d0 = vi_get_x(vd);
        float d1 = vi_get_y(vd);
        float d2 = vi_get_z(vd);

        float s = core::max(ml::abs(d0), core::max(ml::abs(d1), ml::abs(d2)));
        s = (s != 0.0f) ? s : 1.0f;

        d0 /= s; d1 /= s; d2 /= s;

        //Final adjustments to make inflection point polynomial
        ml::vec2   r[2] = { {1.0f, 0.0f}, {1.0f, 0.0f} };

        v128 F0, F1, F2, F3;
        float t0, s0, t1, s1, t00, t11, s00, s11, t01, s01;

        if (d0 != 0)
        {
            float h[3] = { -d0*d0, d0*d1, d0*d2-d1*d1 }; //Hessian coefficients
            float det  = 4*h[0]*h[2] - h[1]*h[1];        //Evaluate cubic determinant

            if (det >= 0.0f)
            {
                float d[3] = { -3.0f*d0, 3.0f*d1, -d2 };

                solveQuadratic(d, (float*)r);

                t0 = r[0].x; s0 = r[0].y;
                t1 = r[1].x; s1 = r[1].y;

                normSquare(t0, s0);
                normSquare(t1, s1);

                t00 = t0 * t0; s00 = s0 * s0;
                t11 = t1 * t1; s11 = s1 * s1;

                F0 = vi_set( t0*t1,        t00*t0,       t11*t1,      1.0f);
                F1 = vi_set(-t0*s1-t1*s0, -3.0f*t00*s0, -3.0f*t11*s1, 0.0f);
                F2 = vi_set( s0*s1,        3.0f*t0*s00,  3.0f*t1*s11, 0.0f);
                F3 = vi_set( 0.0f,        -s00*s0,      -s11*s1,      0.0f);
            }
            else
            {
                solveQuadratic(h, (float*)r);

                t0 = r[0].x; s0 = r[0].y;
                t1 = r[1].x; s1 = r[1].y;

                normSquare(t0, s0);
                normSquare(t1, s1);

                t00 = t0 * t0; s00 = s0 * s0;
                t01 = t0 * t1; s01 = s0 * s1;
                t11 = t1 * t1; s11 = s1 * s1;

                F0 = vi_set( t01,          t00*t1,              t0*t11,             1.0f);
                F1 = vi_set(-t0*s1-t1*s0, -t00*s1-2.0f*t01*s0, -t11*s0-2.0f*t01*s1, 0.0f);
                F2 = vi_set( s01,          t1*s00+2.0f*t0*s01,  t0*s11+2.0f*t1*s01, 0.0f);
                F3 = vi_set( 0.0f,        -s00*s1,             -s0*s11,             0.0f);
            }
        }
        else if (d1 != 0.0f)
        {
            t0 = d2; s0 = 3.0f*d1;

            normSquare(t0, s0);

            t00 = t0 * t0; s00 = s0 * s0;

            F0 = vi_set( t0,    t00*t0,      1.0f, 1.0f);
            F1 = vi_set(-s0,   -3.0f*t00*s0, 0.0f, 0.0f);
            F2 = vi_set( 0.0f,  3.0f*t0*s00, 0.0f, 0.0f);
            F3 = vi_set( 0.0f, -s00*s0,      0.0f, 0.0f);
        }
        else if (d2 != 0.0f)
        {
            F0 = vi_set(0.0f, 0.0f, 0.0f, 0.0f);
            F1 = vi_set(1.0f, 0.0f, 1.0f, 0.0f);
            F2 = vi_set(0.0f, 1.0f, 0.0f, 0.0f);
            F3 = vi_set(0.0f, 0.0f, 0.0f, 0.0f);
        }
        else
        {
            F0 = vi_set_zero();
            F1 = vi_set_zero();
            F2 = vi_set_zero();
            F3 = vi_set_zero();
        }

        klm[0] = F0;
        klm[1] = vi_mad(F1, vi_set_all(0.3333333333f), F0);
        klm[2] = vi_mad(F2, vi_set_all(0.3333333333f), vi_mad(F1, vi_set_all(0.6666666667f), F0));
        klm[3] = vi_add(F3, vi_add(F2, vi_add(F1, F0)));

        subdPtCount = 0;
        for (int i=0; i<2; i++)
        {
            if (r[i].x<0)
            {
                r[i].x = -r[i].x;
                r[i].y = -r[i].y;
            }

            if (0<r[i].x && r[i].x<r[i].y)
            {
                subdPts[subdPtCount++] = r[i].x/r[i].y;
            }
        }
    }

    float calcImplicit(const ml::vec3& tc)
    {
        return tc.x*tc.x*tc.x - tc.y*tc.z;
    }

    inline void changeOrient(ml::vec3& tc)
    {
        tc.x = -tc.x;
        tc.y = -tc.y;
    }

    //Function only works on simple cubic subcurve - which does
    //not change curvature direction on oposite anywhere it is defined
    void correctOrient(ml::vec3 tc[4])
    {
        float valueCP1 = calcImplicit(tc[1]);
        float valueCP2 = calcImplicit(tc[2]);

        //TODO: Test following
        float valueToCheck = ml::abs(valueCP1)>ml::abs(valueCP2)?valueCP1:valueCP2;
        if (valueToCheck<0)
        //Test fails if two non-curve control points are outside but curve value is close to zero 
        //if (ml::lessThenE(valueCP1, 0) || ml::lessThenE(valueCP2, 0))
        //Test fails if one of the control points coincides with start or
        //end control point but has a different sign due to a rounding error
        //if (valueCP1<0 || valueCP2<0)
        {
            changeOrient(tc[0]);
            changeOrient(tc[1]);
            changeOrient(tc[2]);
            changeOrient(tc[3]);
        }
    }

    void meshAddBezier3(geometry_t* geom, uint16_t prevIdx, uint16_t curIdx, const ml::vec2& cp0, const ml::vec2&  cp1, const ml::vec2& cp2, const ml::vec2& cp3)
    {
        int       count;
        float     subdPts[2];
        v128 vcp0[4] = {vi_load_v2(&cp0), vi_load_v2(&cp1), vi_load_v2(&cp2), vi_load_v2(&cp3)};
        v128 vklm0[4];

        bezier3MakeImplicit(vcp0, vklm0, count, subdPts);

        if (count>1 && subdPts[0]>subdPts[1])
        {
            core::swap(subdPts[0], subdPts[1]);
        }

        ml::vec2 cp[4];
        ml::vec3 klm[4];

        for(int i=0; i<count; ++i)
        {
            v128 vcp1[4];
            v128 vklm1[4];

            float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

            subdivide(vcp0,  t, vcp1,  vcp0);
            subdivide(vklm0, t, vklm1, vklm0);

            vi_store_v2(&cp[0], vcp1[0]);
            vi_store_v2(&cp[1], vcp1[1]);
            vi_store_v2(&cp[2], vcp1[2]);
            vi_store_v2(&cp[3], vcp1[3]);

            vi_store_v3(&klm[0], vklm1[0]);
            vi_store_v3(&klm[1], vklm1[1]);
            vi_store_v3(&klm[2], vklm1[2]);
            vi_store_v3(&klm[3], vklm1[3]);

            correctOrient(klm);
            geomAddB3Vertices(geom, cp, klm);

            //Carefully, we changed places of subdivided curves,
            //that's why suitable points are 0 and 7
            uint16_t idx = geomAddVertex(geom, cp[3]);
            geomAddTri(geom, prevIdx, idx, curIdx);
            prevIdx = idx;
        }

        vi_store_v2(&cp[0], vcp0[0]);
        vi_store_v2(&cp[1], vcp0[1]);
        vi_store_v2(&cp[2], vcp0[2]);
        vi_store_v2(&cp[3], vcp0[3]);

        vi_store_v3(&klm[0], vklm0[0]);
        vi_store_v3(&klm[1], vklm0[1]);
        vi_store_v3(&klm[2], vklm0[2]);
        vi_store_v3(&klm[3], vklm0[3]);

        correctOrient(klm);
        geomAddB3Vertices(geom, cp, klm);
    }

    void stencilPath(path_data_t* path, int useAA)
    {
        gfx::setMVP();

        if (path->numIndices > 0)
        {
            gfx::setStdProgram(0);
            glBindVertexArray(vf::p2_vertex_t::vao);
            glBindVertexBuffer(0, gfx_res::buffer, 0, sizeof(vf::p2_vertex_t));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx_res::buffer);
            glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)path->numIndices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(path->offsetIndices), path->baseVertex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        if (path->numB3Indices > 0)
        {
            glUseProgram(useAA ? gfx_res::prgRasterCubicAA : gfx_res::prgRasterCubic);
            glBindVertexArray(vf::p2uv3_vertex_t::vao);
            glBindVertexBuffer(0, gfx_res::buffer, 0, sizeof(vf::p2uv3_vertex_t));
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gfx_res::buffer);
            glDrawElementsBaseVertex(GL_TRIANGLES, (GLsizei)path->numB3Indices, GL_UNSIGNED_SHORT, BUFFER_OFFSET(path->offsetB3Indices), path->baseB3Vertex);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glBindVertexArray(0);
    }

    void getPathBounds(Path path, float& x1, float& y1, float& x2, float& y2)
    {
        x1 = path->xmin; y1 = path->ymin;
        x2 = path->xmax; y2 = path->ymax;
    }

    Path createPath(size_t numCmd, const VGubyte* cmd, size_t numData, const VGfloat* data)
    {
        const size_t maxIndices    = numCmd * 9;
        const size_t maxVertices   = numCmd * 5;
        const size_t maxB3Vertices = numCmd * 10;

        geometry_t pathGeom = {
            (uint16_t*)core::thread_stack_alloc(sizeof(uint16_t)*maxIndices),
            (vf::p2_vertex_t*) core::thread_stack_alloc(sizeof(vf::p2_vertex_t)*maxVertices),
            (vf::p2uv3_vertex_t*) core::thread_stack_alloc(sizeof(vf::p2uv3_vertex_t)*maxB3Vertices),
            0, 0, 0
        };

        ml::vec2  cp0 = {0.0f, 0.0f},
                  cp1 = {0.0f, 0.0f},
                  o   = {0.0f, 0.0f},
                  p   = {0.0f, 0.0f};
        bool      isContourStarted = false;
        uint16_t  startIdx=0, prevIdx=0, curIdx=0; //assign default values to make compiler happy
        memory_t  mem = {(uint8_t*)data, numData*sizeof(float), 0};

        for (size_t s=0; s<numCmd; ++s)
        {
            int      segment = cmd[s]&0x1E;
            ml::vec2 origin = (cmd[s] & 1) ? o : ml::vec2{0.0f, 0.0f};

            if (segment==VG_CLOSE_PATH)
            {
                isContourStarted = false;
            }
            else if (segment==VG_MOVE_TO)
            {
                isContourStarted = false;
                o = p = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
            }
            else
            {
                //Here starts non control commands
                //So we can handle start path case
                if (!isContourStarted)
                {
                    isContourStarted = true;
                    startIdx = prevIdx = curIdx = geomAddVertex(&pathGeom, o);
                }

                switch (segment)
                {
                    case VG_LINE_TO:
                        o = p = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        break;

                    case VG_HLINE_TO:
                        o.x = p.x = mem_as_value_advance<float>(mem.buffer, mem.allocated) + origin.x;
                        break;

                    case VG_VLINE_TO:
                        o.y = p.y = mem_as_value_advance<float>(mem.buffer, mem.allocated) + origin.y;
                        break;

                    case VG_QUAD_TO:
                        p = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        o = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        break;

                    case VG_SQUAD_TO:
                        p = 2.0f*o - p;
                        o = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        break;

                    case VG_CUBIC_TO:
                        cp0 = o;
                        cp1 = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        p   = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        o   = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        break;

                    case VG_SCUBIC_TO:
                        cp0 = o;
                        cp1 = 2.0f*o - p;
                        p   = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        o   = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;
                        break;

                    case VG_SCWARC_TO:
                    case VG_SCCWARC_TO:
                    case VG_LCWARC_TO:
                    case VG_LCCWARC_TO:
                        {
                            ml::vec2 r, c;
                            float a;

                            r = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated);
                            a = mem_as_value_advance<float>(mem.buffer, mem.allocated);
                            c = mem_as_value_advance<ml::vec2>(mem.buffer, mem.allocated) + origin;

                            o = p = c;
                        }
                        break;

                    default:
                        assert(0);
                }

                assert(isContourStarted);
                prevIdx = curIdx;
                curIdx = geomAddVertex(&pathGeom, o);
                geomAddTri(&pathGeom, startIdx, prevIdx, curIdx);

                switch (segment)
                {
                    case VG_CUBIC_TO:
                    case VG_SCUBIC_TO:
                        meshAddBezier3(&pathGeom, prevIdx, curIdx, cp0, cp1, p, o);
                        break;

                    case VG_QUAD_TO:
                    case VG_SQUAD_TO:
                        //TODO: implement quadratic curves
                        assert(0);
                        break;

                    case VG_SCWARC_TO:
                    case VG_SCCWARC_TO:
                    case VG_LCWARC_TO:
                    case VG_LCCWARC_TO:
                        //TODO: implement elliptic arcs
                        assert(0);
                        break;
                };
            }
        }

        assert(pathGeom.numIndices    <= maxIndices);
        assert(pathGeom.numVertices   <= maxVertices);
        assert(pathGeom.numB3Vertices <= maxB3Vertices);

        Path path = geomToPath(&pathGeom);

        core::thread_stack_reset(pathGeom.indices);

        return path;
    }

    void destroyPath(Path path)
    {
        etlsf_free_range(gfx_res::vgGArena, path->gpuMemHandle);
        mem::free(gfx::memArena, path);
    }
}