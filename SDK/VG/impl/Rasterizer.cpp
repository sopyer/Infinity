#include <cassert>
#include <opengl.h>
#include <utils.h>

#include "SharedResources.h"
#include "Rasterizer.h"

#include <cassert>
#include <algorithm>
#include <utils.h>

namespace impl
{
    enum
    {
        RATIONAL_LOOP_CUBIC,
        RATIONAL_SERPENTINE_CUSP_CUBIC,
        INTEGRAL_LOOP_CUBIC,
        INTEGRAL_SERPENTINE_CUSP_CUBIC,
        CUSP_AT_INFINITY_CUBIC,
        RATIONAL_QUADRATIC_CUBIC,
        DEGENERATE_CUBIC
    };

    template<typename T>
    void subdivide(const T cubic[4], float t, T subCubic1[4], T subCubic2[4])
    {
        T p0 = cubic[0];
        T p3 = cubic[3];

        T p01 = glm::mix(cubic[0], cubic[1], t);
        T p12 = glm::mix(cubic[1], cubic[2], t);
        T p23 = glm::mix(cubic[2], cubic[3], t);

        T p012 = glm::mix(p01, p12, t);
        T p123 = glm::mix(p12, p23, t);

        T p0123 = glm::mix(p012, p123, t);

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
        float D = b*b-4*a*c;

        assert(D>=0);

        if (D==0)
        {
            r[0] = r[2] = -b;
            r[1] = r[3] = 2*a;
        }
        else
        {
            float rr = -b + (b<0?1:-1)*sqrt(D);

            r[0] = rr;  r[1] = 2*a;
            r[2] = 2*c; r[3] = rr;
        }
    }

    void normSquare(float* x, float* y)
    {
        float s;

        s = ut::max(ml::abs(*x), ml::abs(*y));
        s = s > ML_C_EPS7 ? s : 1.0f;

        *x /= s; *y /= s;
    }

    void bezier3MakeImplicit(glm::vec2 pos[4], glm::vec3 klm[4], int& subdPtCount, float* subdPts)
    {
        glm::vec2 bb[3] = { pos[1]-pos[0], pos[2]-pos[0], pos[3]-pos[0] };

        //Transform from Bezier to power basis
        glm::vec2 pb[3] = { 3.0f*bb[0], -6.0f*bb[0] +  3.0f*bb[1], 3.0f*bb[0] + -3.0f*bb[1] + bb[2] };

        float d0 =  pb[1].x*pb[2].y - pb[1].y*pb[2].x;
        float d1 =  pb[2].x*pb[0].y - pb[2].y*pb[0].x;
        float d2 =  pb[0].x*pb[1].y - pb[0].y*pb[1].x;

        //Mitigates precision issues
        d0 = ml::abs(d0) > ML_C_EPS7 ? d0 : 0.0f;
        d1 = ml::abs(d1) > ML_C_EPS7 ? d1 : 0.0f;
        d2 = ml::abs(d2) > ML_C_EPS7 ? d2 : 0.0f;

        float s = ut::max(ml::abs(d0), ut::max(ml::abs(d1), ml::abs(d2)));
        s = (s != 0.0f) ? s : 1.0f;

        d0 /= s;
        d1 /= s;
        d2 /= s;

        //Hessian coefficients
        float h[3] = { -d0*d0, d0*d1, d0*d2-d1*d1 };

        //Evaluate cubic determinant
        float det = 4*h[0]*h[2] - h[1]*h[1];

        //det>0 - sepentine, det==0 - cusp, det<0 - loop
        int subCubicType = !!(det>=0);
        int cubicType = d0 !=0 ? INTEGRAL_LOOP_CUBIC+subCubicType:
                        d1 !=0 ? CUSP_AT_INFINITY_CUBIC:
                        d2 !=0 ? RATIONAL_QUADRATIC_CUBIC:DEGENERATE_CUBIC;

        //Final adjustments to make inflection point polynomial
        float d[3] = { -3.0f*d0, 3.0f*d1, -d2 };

        glm::vec2   r[2] = 
        {
            glm::vec2(1, 0),
            glm::vec2(1, 0),
        };

        glm::mat4   k;

        v128 F0, F1, F2, F3;
        float t0, s0, t1, s1, t00, t11, s00, s11, t01, s01;

        assert(cubicType!=RATIONAL_LOOP_CUBIC && cubicType!=RATIONAL_SERPENTINE_CUSP_CUBIC);
        switch (cubicType)
        {
        case INTEGRAL_LOOP_CUBIC:
            solveQuadratic(h, (float*)r);

            t0 = r[0].x; s0 = r[0].y;
            t1 = r[1].x; s1 = r[1].y;

            normSquare(&t0, &s0);
            normSquare(&t1, &s1);

            t00 = t0 * t0; s00 = s0 * s0;
            t01 = t0 * t1; s01 = s0 * s1;
            t11 = t1 * t1; s11 = s1 * s1;

            F0 = vi_set( t01,          t00*t1,              t0*t11,             1.0f);
            F1 = vi_set(-t0*s1-t1*s0, -t00*s1-2.0f*t01*s0, -t11*s0-2.0f*t01*s1, 0.0f);
            F2 = vi_set( s01,          t1*s00+2.0f*t0*s01,  t0*s11+2.0f*t1*s01, 0.0f);
            F3 = vi_set( 0.0f,        -s00*s1,             -s0*s11,             0.0f);

            break;

        case INTEGRAL_SERPENTINE_CUSP_CUBIC:
            solveQuadratic(d, (float*)r);

            t0 = r[0].x; s0 = r[0].y;
            t1 = r[1].x; s1 = r[1].y;

            normSquare(&t0, &s0);
            normSquare(&t1, &s1);

            t00 = t0 * t0; s00 = s0 * s0;
            t11 = t1 * t1; s11 = s1 * s1;

            F0 = vi_set( t0*t1,        t00*t0,       t11*t1,      1.0f);
            F1 = vi_set(-t0*s1-t1*s0, -3.0f*t00*s0, -3.0f*t11*s1, 0.0f);
            F2 = vi_set( s0*s1,        3.0f*t0*s00,  3.0f*t1*s11, 0.0f);
            F3 = vi_set( 0.0f,        -s00*s0,      -s11*s1,      0.0f);

            break;

        case CUSP_AT_INFINITY_CUBIC:
            t0 = -d[2]; s0 = d[1];

            normSquare(&t0, &s0);

            t00 = t0 * t0; s00 = s0 * s0;

            F0 = vi_set( t0,    t00*t0,      1.0f, 1.0f);
            F1 = vi_set(-s0,   -3.0f*t00*s0, 0.0f, 0.0f);
            F2 = vi_set( 0.0f,  3.0f*t0*s00, 0.0f, 0.0f);
            F3 = vi_set( 0.0f, -s00*s0,      0.0f, 0.0f);

            break;

        case RATIONAL_QUADRATIC_CUBIC:
            F0 = vi_set(0.0f, 0.0f, 0.0f, 0.0f);
            F1 = vi_set(1.0f, 0.0f, 1.0f, 0.0f);
            F2 = vi_set(0.0f, 1.0f, 0.0f, 0.0f);
            F3 = vi_set(0.0f, 0.0f, 0.0f, 0.0f);

            break;

        case DEGENERATE_CUBIC:
            F0 = vi_set_0000();
            F1 = vi_set_0000();
            F2 = vi_set_0000();
            F3 = vi_set_0000();

            break;

        default:
            assert(false && "Algorithm failed to determine correct cubic curve");
        }

        vi_store_v3(&klm[0], F0);
        vi_store_v3(&klm[1], vi_mad(F1, vi_set_ffff(0.3333333333f), F0));
        vi_store_v3(&klm[2], vi_mad(F2, vi_set_ffff(0.3333333333f), vi_mad(F1, vi_set_ffff(0.6666666667f), F0)));
        vi_store_v3(&klm[3], vi_add(F3, vi_add(F2, vi_add(F1, F0))));

        subdPtCount = 0;
        for (int i=0; i<2; i++)
        {
            r[i] = (r[i].x<0)?-r[i]:r[i];

            if (!(0<r[i].x && r[i].x<r[i].y))
                continue;

            subdPts[subdPtCount++] = r[i].x/r[i].y;
        }
    }

    glm::vec2 readVec2(const VGfloat*& ptr)
    {
        glm::vec2 res(*ptr, *(ptr+1));
        ptr+=2;
        return res;
    }

    float readFloat(const VGfloat*& ptr)
    {
        float res = *ptr;
        ++ptr;
        return res;
    }

    float calcImplicit(const glm::vec3& tc)
    {
        return tc.x*tc.x*tc.x - tc.y*tc.z;
    }

    inline void changeOrient(glm::vec3& tc)
    {
        tc.x = -tc.x;
        tc.y = -tc.y;
    }

    //Function only works on simple cubic subcurve - which does
    //not change curvature direction on oposite anywhere it is defined
    void correctOrient(glm::vec3 tc[4])
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

    void meshAddBezier3(Geometry& geom, u16 prevIdx, u16 curIdx, const glm::vec2& cp0, const glm::vec2&  cp1, const glm::vec2& cp2, const glm::vec2& cp3)
    {
        glm::vec2 ctrlPt[4*2] = {cp0, cp1, cp2, cp3};
        glm::vec3 klm[4*2];
        int       count;
        float     subdPts[2];

        bezier3MakeImplicit(ctrlPt, klm, count, subdPts);

        if (count>1 && subdPts[0]>subdPts[1])
        {
            ut::swap(subdPts[0], subdPts[1]);
        }

        for(int i=0; i<count; ++i)
        {
            float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

            subdivide(ctrlPt, t, ctrlPt+4, ctrlPt);
            subdivide(klm, t, klm+4, klm);

            correctOrient(klm+4);
            geom.bezier3AddVertices(ctrlPt+4, klm+4);

            //Carefully, we changed places of subdivided curves,
            //that's why suitable points are 0 and 7
            u16 idx = geom.shapeAddVertex(ctrlPt[0]);
            geom.shapeAddTri(prevIdx, idx, curIdx);
            prevIdx = idx;
        }

        correctOrient(klm);
        geom.bezier3AddVertices(ctrlPt, klm);
    }

    void buildFillGeometry(Geometry&      geom,
                           const VGint    count,
                           const VGubyte* commands,
                           const VGfloat* pathData)
    {
        glm::vec2 cp0, cp1, o, p;
        bool      isContourStarted = false;
        u16       startIdx=0, prevIdx=0, curIdx=0; //assign default values to make compiler happy

        for (VGint s=0; s<count; ++s)
        {
            int  segment  = commands[s]&0x1E;
            bool rel      = commands[s]&1;

            if (segment==VG_CLOSE_PATH)
            {
                isContourStarted = false;
            }
            else if (segment==VG_MOVE_TO)
            {
                isContourStarted = false;
                o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
            }
            else
            {
                //Here starts non control commands
                //So we can handle start path case
                if (!isContourStarted)
                {
                    isContourStarted = true;
                    startIdx = prevIdx = curIdx = geom.shapeAddVertex(o);
                }

                switch (segment)
                {
                    case VG_LINE_TO:
                        o = p = readVec2(pathData) + (rel?o:glm::vec2(0));
                        break;

                    case VG_HLINE_TO:
                        o.x = p.x = readFloat(pathData) + (rel?o.x:0);
                        break;

                    case VG_VLINE_TO:
                        o.y = p.y = readFloat(pathData) + (rel?o.y:0);
                        break;

                    case VG_QUAD_TO:
                        p = readVec2(pathData) + (rel?o:glm::vec2(0));
                        o = readVec2(pathData) + (rel?o:glm::vec2(0));
                        break;

                    case VG_SQUAD_TO:
                        p = 2.0f*o - p;
                        o = readVec2(pathData) + (rel?o:glm::vec2(0));
                        break;

                    case VG_CUBIC_TO:
                        cp0 = o;
                        cp1 = readVec2(pathData) + (rel?o:glm::vec2(0));
                        p = readVec2(pathData) + (rel?o:glm::vec2(0));
                        o = readVec2(pathData) + (rel?o:glm::vec2(0));
                        break;

                    case VG_SCUBIC_TO:
                        cp0 = o;
                        cp1 = 2.0f*o - p;
                        p = readVec2(pathData) + (rel?o:glm::vec2(0));
                        o = readVec2(pathData) + (rel?o:glm::vec2(0));
                        break;

                    case VG_SCWARC_TO:
                    case VG_SCCWARC_TO:
                    case VG_LCWARC_TO:
                    case VG_LCCWARC_TO:
                        {
                            glm::vec2 r = readVec2(pathData);
                            float     a = readFloat(pathData);
                            glm::vec2 e = readVec2(pathData) + (rel?o:glm::vec2(0));

                            o = p = e;
                        }
                        break;

                    default:
                        assert(0);
                }

                assert(isContourStarted);
                prevIdx = curIdx;
                curIdx = geom.shapeAddVertex(o);
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
            glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom.shapeVertices.begin());
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
            glVertexPointer(2, GL_FLOAT, sizeof(glm::vec2), geom->shapeVertices.begin());
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
