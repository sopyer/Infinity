#include <opengl.h>
#include <utils.h>

#include "SharedResources.h"
#include "Rasterizer.h"

namespace impl
{
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
            float rr = -b + (b < 0 ? 1 : -1)*sqrt(D);

            r[0] = rr;  r[1] = 2*a;
            r[2] = 2*c; r[3] = rr;
        }
    }

    void normSquare(float& x, float& y)
    {
        float s;

        s = ut::max(ml::abs(x), ml::abs(y));
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
        v128 pb0 = vi_mul(vi_set_ffff( 3.0f), bb0);
        v128 pb1 = vi_mad(vi_set_ffff(-6.0f), bb0, vi_mul(vi_set_ffff( 3.0f), bb1));
        v128 pb2 = vi_mad(vi_set_ffff( 3.0f), bb0, vi_mad(vi_set_ffff(-3.0f), bb1, bb2));

        // make xxx and yyy
        v128 xy[2];
        v128 m[4] = {pb0, pb1, pb2, vi_set_0000()};
        ml::transpose_mat4x2(xy, m);

        //d0 =  pb[1].x*pb[2].y - pb[1].y*pb[2].x;
        //d1 =  pb[2].x*pb[0].y - pb[2].y*pb[0].x;
        //d2 =  pb[0].x*pb[1].y - pb[0].y*pb[1].x;
        v128 vd = vi_cross3(xy[0], xy[1]);

        // Zero values less then epsilon in order 
        // to mitigates precision issues
        v128 mask = vi_cmp_gt(vi_abs(vd), vi_set_ffff(ML_C_EPS7));
        vd = vi_and(vd, mask);

        float d0 = vi_get_x(vd);
        float d1 = vi_get_y(vd);
        float d2 = vi_get_z(vd);

        float s = ut::max(ml::abs(d0), ut::max(ml::abs(d1), ml::abs(d2)));
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
            F0 = vi_set_0000();
            F1 = vi_set_0000();
            F2 = vi_set_0000();
            F3 = vi_set_0000();
        }

        klm[0] = F0;
        klm[1] = vi_mad(F1, vi_set_ffff(0.3333333333f), F0);
        klm[2] = vi_mad(F2, vi_set_ffff(0.3333333333f), vi_mad(F1, vi_set_ffff(0.6666666667f), F0));
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

    void meshAddBezier3(Geometry& geom, u16 prevIdx, u16 curIdx, const ml::vec2& cp0, const ml::vec2&  cp1, const ml::vec2& cp2, const ml::vec2& cp3)
    {
        int       count;
        float     subdPts[2];
        v128 vcp0[4] = {vi_load_v2(&cp0), vi_load_v2(&cp1), vi_load_v2(&cp2), vi_load_v2(&cp3)};
        v128 vklm0[4];

        bezier3MakeImplicit(vcp0, vklm0, count, subdPts);

        if (count>1 && subdPts[0]>subdPts[1])
        {
            ut::swap(subdPts[0], subdPts[1]);
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
            geom.bezier3AddVertices((glm::vec2*)cp, (glm::vec3*)klm);

            //Carefully, we changed places of subdivided curves,
            //that's why suitable points are 0 and 7
            u16 idx = geom.shapeAddVertex(*(glm::vec2*)&cp[3]);
            geom.shapeAddTri(prevIdx, idx, curIdx);
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
        geom.bezier3AddVertices((glm::vec2*)cp, (glm::vec3*)klm);
    }

    void buildFillGeometry(Geometry& geom, size_t numCmd, const VGubyte* cmd, size_t numData, const VGfloat* data)
    {
        ml::vec2  cp0 = {0.0f, 0.0f},
                  cp1 = {0.0f, 0.0f},
                  o   = {0.0f, 0.0f},
                  p   = {0.0f, 0.0f};
        bool      isContourStarted = false;
        u16       startIdx=0, prevIdx=0, curIdx=0; //assign default values to make compiler happy
        memory_t  mem = {(uint8_t*)data, numData*sizeof(float), 0};

        for (size_t s=0; s<numCmd; ++s)
        {
            int      segment = cmd[s]&0x1E;
            ml::vec2 origin  = (cmd[s]&1) ? o : ml::make_vec2(0.0f, 0.0f);

            if (segment==VG_CLOSE_PATH)
            {
                isContourStarted = false;
            }
            else if (segment==VG_MOVE_TO)
            {
                isContourStarted = false;
                o = p = mem_read<ml::vec2>(&mem) + origin;
            }
            else
            {
                //Here starts non control commands
                //So we can handle start path case
                if (!isContourStarted)
                {
                    isContourStarted = true;
                    startIdx = prevIdx = curIdx = geom.shapeAddVertex(*(glm::vec2*)&o);
                }

                switch (segment)
                {
                    case VG_LINE_TO:
                        o = p = mem_read<ml::vec2>(&mem) + origin;
                        break;

                    case VG_HLINE_TO:
                        o.x = p.x = mem_read<float>(&mem) + origin.x;
                        break;

                    case VG_VLINE_TO:
                        o.y = p.y = mem_read<float>(&mem) + origin.y;
                        break;

                    case VG_QUAD_TO:
                        p = mem_read<ml::vec2>(&mem) + origin;
                        o = mem_read<ml::vec2>(&mem) + origin;
                        break;

                    case VG_SQUAD_TO:
                        p = 2.0f*o - p;
                        o = mem_read<ml::vec2>(&mem) + origin;
                        break;

                    case VG_CUBIC_TO:
                        cp0 = o;
                        cp1 = mem_read<ml::vec2>(&mem) + origin;
                        p   = mem_read<ml::vec2>(&mem) + origin;
                        o   = mem_read<ml::vec2>(&mem) + origin;
                        break;

                    case VG_SCUBIC_TO:
                        cp0 = o;
                        cp1 = 2.0f*o - p;
                        p   = mem_read<ml::vec2>(&mem) + origin;
                        o   = mem_read<ml::vec2>(&mem) + origin;
                        break;

                    case VG_SCWARC_TO:
                    case VG_SCCWARC_TO:
                    case VG_LCWARC_TO:
                    case VG_LCCWARC_TO:
                        {
                            ml::vec2 r, c;
                            float a;

                            r = mem_read<ml::vec2>(&mem);
                            a = mem_read<float>(&mem);
                            c = mem_read<ml::vec2>(&mem) + origin;

                            o = p = c;
                        }
                        break;

                    default:
                        assert(0);
                }

                assert(isContourStarted);
                prevIdx = curIdx;
                curIdx = geom.shapeAddVertex(*(glm::vec2*)&o);
                geom.shapeAddTri(startIdx, prevIdx, curIdx);

                switch (segment)
                {
                    case VG_CUBIC_TO:
                    case VG_SCUBIC_TO:
                        meshAddBezier3(geom, prevIdx, curIdx, cp0, cp1, p, o);
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

        geom.bezier3GenIndices();

        //stupid initialization
        if (!geom.shapeVertices.empty())
        {
            geom.xmin = geom.xmax = geom.shapeVertices[0].x;
            geom.ymin = geom.ymax = geom.shapeVertices[0].y;
        }
        else if (!geom.bezier3Vertices.empty())
        {
            geom.xmin = geom.xmax = geom.bezier3Vertices[0].x;
            geom.ymin = geom.ymax = geom.bezier3Vertices[0].y;
        }
        else
        {
            geom.xmin = geom.ymin = geom.xmax = geom.ymax = 0;
            return;
        }

        for(size_t i=0; i<geom.shapeVertices.size(); ++i)
        {
            geom.xmin = ut::min(geom.xmin, geom.shapeVertices[i].x);
            geom.ymin = ut::min(geom.ymin, geom.shapeVertices[i].y);
            geom.xmax = ut::max(geom.xmax, geom.shapeVertices[i].x);
            geom.ymax = ut::max(geom.ymax, geom.shapeVertices[i].y);
        }

        for(size_t i=0; i<geom.bezier3Vertices.size(); ++i)
        {
            geom.xmin = ut::min(geom.xmin, geom.bezier3Vertices[i].x);
            geom.ymin = ut::min(geom.ymin, geom.bezier3Vertices[i].y);
            geom.xmax = ut::max(geom.xmax, geom.bezier3Vertices[i].x);
            geom.ymax = ut::max(geom.ymax, geom.bezier3Vertices[i].y);
        }
    }

    void rasterizeEvenOddAA(Geometry& geom)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glDisable(GL_STENCIL_TEST);
        glEnable(GL_BLEND);
        glBlendEquation(GL_FUNC_ADD);
        glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_TRUE);

        glEnableClientState(GL_VERTEX_ARRAY);

        if (!geom.shapeIndices.empty())
        {
            glUseProgram(0);
            glColor4ub(0, 0, 0, 255);
            glVertexPointer(2, GL_FLOAT, sizeof(ml::vec2), geom.shapeVertices.begin());
            glDrawElements(GL_TRIANGLES, (GLsizei)geom.shapeIndices.size(), GL_UNSIGNED_SHORT, geom.shapeIndices.begin());
        }

        glClientActiveTexture(GL_TEXTURE0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        if (!geom.bezier3Indices.empty())
        {
            glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].x);
            glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom.bezier3Vertices[0].k);
            glUseProgram(stencilCubicAreaAAProgram);
            glDrawElements(GL_TRIANGLES, (GLsizei)geom.bezier3Indices.size(), GL_UNSIGNED_SHORT, geom.bezier3Indices.begin());
        }

        glPopClientAttrib();
        glPopAttrib();
    }

    void stencilPath(Geometry* geom, int useAA, int useNonZero)
    {
        glPushAttrib(GL_ALL_ATTRIB_BITS);
        glPushClientAttrib(GL_CLIENT_ALL_ATTRIB_BITS);

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glEnable(GL_STENCIL_TEST);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
        glStencilMask(0xFF); //TODO: make mask configurable
        glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_KEEP, useNonZero?GL_INCR_WRAP:GL_INVERT);
        glStencilOpSeparate(GL_BACK,  GL_KEEP, GL_KEEP, useNonZero?GL_DECR_WRAP:GL_INVERT);

        glEnableClientState(GL_VERTEX_ARRAY);

        if (!geom->shapeIndices.empty())
        {
            glUseProgram(0);
            glVertexPointer(2, GL_FLOAT, sizeof(ml::vec2), geom->shapeVertices.begin());
            glDrawElements(GL_TRIANGLES, (GLsizei)geom->shapeIndices.size(), GL_UNSIGNED_SHORT, geom->shapeIndices.begin());
        }

        if (useAA) glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

        glClientActiveTexture(GL_TEXTURE0);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        if (!geom->bezier3Indices.empty())
        {
            glVertexPointer(2, GL_FLOAT, sizeof(Bezier3Vertex), &geom->bezier3Vertices[0].x);
            glTexCoordPointer(3, GL_FLOAT, sizeof(Bezier3Vertex), &geom->bezier3Vertices[0].k);
            glUseProgram(useAA?stencilCubicAreaAAProgram:stencilCubicAreaProgram);
            glDrawElements(GL_TRIANGLES, (GLsizei)geom->bezier3Indices.size(), GL_UNSIGNED_SHORT, geom->bezier3Indices.begin());
        }

        if (useAA) glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

        glPopClientAttrib();
        glPopAttrib();
    }
}
