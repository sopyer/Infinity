#define _CRT_SECURE_NO_WARNINGS

#include <fwk/fwk.h>

#include "VGExp.h"
#include "Math.h"
#include <gfx/Path.h>

//float w1 = -0.35f;
//float w2 = -0.35f;
//glm::vec3	controlPts[4] = {glm::vec3(0, 0, 1), glm::vec3(10*w1, 10*w1, w1), glm::vec3(0*w2, 10*w2, w2), glm::vec3(10, 0, 1)};
glm::vec2	controlPts2[4] = {glm::vec2(0, 0), glm::vec2(10.0f, 10.0f), glm::vec2(0, 10), glm::vec2(10, 0)};
//glm::vec2	controlPts2[4] = {glm::vec2(10, 0), glm::vec2(20.0f, 40.0f), glm::vec2(40, 40), glm::vec2(50, 0)};
//float w1 = /*1.0f/*/6.0f, w2 = 1.0f/*/3.0f*/;
//glm::vec3 controlPts[4] = {
//	glm::vec3(-10.0f,     0.0f,    1.0f),
//	glm::vec3(-10.0f*w1, 20.0f*w1,   w1),
//	glm::vec3( 10.0f*w2, 20.0f*w2,   w2),
//	glm::vec3( 10.0f,     0.0f,    1.0f),
//};

//glm::vec3 controlPts[4] = {
//	glm::vec3(-10.0f,     0.0f,    1.0f),
//	glm::vec3( 0.0f*w1,  10.0f*w1,   w1),
//	glm::vec3( 10.0f*w2,  0.0f*w2,   w2),
//	glm::vec3( 30.0f,    20.0f,    1.0f),
//};

//float w1 = 1.0f/3.0f, w2 = 1.0f/3.0f;
//glm::vec3 controlPts[4] = {
//	glm::vec3(-10.0f,     0.0f,    1.0f),
//	glm::vec3(-10.0f*w1, 20.0f*w1,   w1),
//	glm::vec3( 10.0f*w2, 20.0f*w2,   w2),
//	glm::vec3( 10.0f,     0.0f,    1.0f),
//};

glm::vec3 controlPts[4] = {
    glm::vec3(-20.0f,  0.0f, 1.0f),
    glm::vec3( 30.0f, 40.0f, 1.0f),
    glm::vec3(-30.0f, 40.0f, 1.0f),
    glm::vec3( 20.0f,  0.0f, 1.0f),
};

Geometry<RCubicVertex>  rcubic;
Geometry<glm::vec2> rtri;

void addCubic(glm::vec3 cp[4], Geometry<RCubicVertex>& cubicGeom, Geometry<glm::vec2>& triGeom)
{
    glm::vec3   bezierBasisCP[4*4];
    glm::vec4   klmn[4*4];
    int         count;
    float       subdPt[3];

    memcpy(bezierBasisCP, cp, 4*sizeof(glm::vec3));
    implicitizeRationalBezierCubic(bezierBasisCP, klmn, count, subdPt);

    glm::vec2   tri[4] = {
        cp[0]._xy()/cp[0].z,
        cp[1]._xy()/cp[1].z,
        cp[2]._xy()/cp[2].z,
        cp[3]._xy()/cp[3].z,
    };

    glm::vec3 bary;
    glm::vec2 pt;

    float ttt[] = {0, 1, 0.5f, 0.75f, 0.25f, 0.333333f, 0.4f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f};
    for (size_t i=0; i<ARRAY_SIZE(ttt); ++i)
    {
        evalRationalBezier(ml::as<float>(cp),  ttt[i], pt);

        calcBarycentric(tri[0], tri[1], tri[3], pt, bary);
        assert(glm::equalEpsilonGTX(pt, bary.x*tri[0]+bary.y*tri[1]+bary.z*tri[3], ml::EPS7));

        glm::vec3 scale = bary/glm::vec3(cp[0].z, cp[1].z, cp[3].z);
        glm::vec4 klmnI = (scale.x*klmn[0] + scale.y*klmn[1] + scale.z*klmn[3])/(scale.x + scale.y + scale.z);

        float f = klmnI.x*klmnI.x*klmnI.x - klmnI.y*klmnI.z*klmnI.w;

        assert(ml::equalE(f, 0, 0.01f));
    }

    glm::vec2   intervals[4] = {glm::vec2(0, 1)};
    glm::vec2*  intervalLast = intervals+4;
    size_t      intervalCount = 1;

    for (int i=0; i<count; i++)
    {
        float   t = subdPt[i];
        int     left = 0;
        int         right = intervalCount++;

        while (left<right && !(ml::lessThenE(intervals[left].x, t) && ml::lessThenE(t, intervals[left].y)))
            ++left;

        if (left==right) continue;

        glm::vec2 oldInterval = intervals[left];

        intervals[left]  = glm::vec2(oldInterval.x, t);
        intervals[right] = glm::vec2(t, oldInterval.y);

        left*=4; right*=4;

        float tt = (t-oldInterval.x)/(oldInterval.y-oldInterval.x);

        cubic::subdivide(bezierBasisCP+left, tt, bezierBasisCP+left, bezierBasisCP+right);
        cubic::subdivide(klmn+left, tt, klmn+left, klmn+right);
    }

    size_t cvertBase = cubicGeom.vertices.size();
    size_t tvertBase = triGeom.vertices.size();

    cubicGeom.vertices.resize(cvertBase+4*intervalCount);

    for (size_t base=0; base<4*intervalCount; base+=4, cvertBase+=4)
    {
        correctOrient(klmn+base);

        cubicGeom.vertices[cvertBase+0].pos = glm::vec4(bezierBasisCP[base+0].x, bezierBasisCP[base+0].y, 0, bezierBasisCP[base+0].z);
        cubicGeom.vertices[cvertBase+0].klmn = klmn[base+0];
        cubicGeom.vertices[cvertBase+1].pos = glm::vec4(bezierBasisCP[base+1].x, bezierBasisCP[base+1].y, 0, bezierBasisCP[base+1].z);
        cubicGeom.vertices[cvertBase+1].klmn = klmn[base+1];
        cubicGeom.vertices[cvertBase+2].pos = glm::vec4(bezierBasisCP[base+2].x, bezierBasisCP[base+2].y, 0, bezierBasisCP[base+2].z);
        cubicGeom.vertices[cvertBase+2].klmn = klmn[base+2];
        cubicGeom.vertices[cvertBase+3].pos = glm::vec4(bezierBasisCP[base+3].x, bezierBasisCP[base+3].y, 0, bezierBasisCP[base+3].z);
        cubicGeom.vertices[cvertBase+3].klmn = klmn[base+3];

        cubicGeom.indices.pushBack(cvertBase+0); cubicGeom.indices.pushBack(cvertBase+1); cubicGeom.indices.pushBack(cvertBase+2);
        cubicGeom.indices.pushBack(cvertBase+0); cubicGeom.indices.pushBack(cvertBase+2); cubicGeom.indices.pushBack(cvertBase+3);

        if (intervalCount==1) break;

        //we assume first iinterval is at the beginning of the curve
        if (base==0)
        {
            triGeom.vertices.pushBack(glm::vec2(bezierBasisCP[base+0].x, bezierBasisCP[base+0].y)/bezierBasisCP[base+0].z);
        }
        else
        {
            triGeom.vertices.pushBack(glm::vec2(bezierBasisCP[base+0].x, bezierBasisCP[base+0].y)/bezierBasisCP[base+0].z);
            triGeom.vertices.pushBack(glm::vec2(bezierBasisCP[base+3].x, bezierBasisCP[base+3].y)/bezierBasisCP[base+3].z);

            triGeom.indices.pushBack(tvertBase); triGeom.indices.pushBack(tvertBase+base/2-1); triGeom.indices.pushBack(tvertBase+base/2);
        }
    }
}

void addAATriangle(glm::vec3 cp[3], Geometry<RCubicVertex>& cubicGeom)
{
    size_t cvertBase = cubicGeom.vertices.size();

    cubicGeom.vertices.resize(cvertBase+3);

    cubicGeom.vertices[cvertBase+0].pos  = glm::vec4(cp[0].x, cp[0].y,    0, cp[0].z);
    cubicGeom.vertices[cvertBase+0].klmn = glm::vec4(   0.0f,    0.0f, 1.0f,    1.0f);
    cubicGeom.vertices[cvertBase+1].pos  = glm::vec4(cp[1].x, cp[1].y,    0, cp[1].z);
    cubicGeom.vertices[cvertBase+1].klmn = glm::vec4(   0.0f,     1.0, 1.0f,    1.0f);
    cubicGeom.vertices[cvertBase+2].pos  = glm::vec4(cp[2].x, cp[2].y,    0, cp[2].z);
    cubicGeom.vertices[cvertBase+2].klmn = glm::vec4(   0.0f,    0.0f, 1.0f,    1.0f);

    cubicGeom.indices.pushBack(cvertBase+0); cubicGeom.indices.pushBack(cvertBase+1); cubicGeom.indices.pushBack(cvertBase+2);
}

void addDistanceTransformCubic(glm::vec3 cp[4], Geometry<RCubicVertex>& distTransGeom)
{
    glm::vec4   klmn[4];
    int         count;
    float       subdPt[3];

    implicitizeRationalBezierCubic(cp, klmn, count, subdPt);

    glm::vec2	tri[4] = {
        cp[0]._xy()/cp[0].z,
        cp[1]._xy()/cp[1].z,
        cp[2]._xy()/cp[2].z,
        cp[3]._xy()/cp[3].z,
    };

    glm::vec3 bary;
    glm::vec2 pt;

    glm::vec2 pts[] = {
        glm::vec2(-10, 0),
        glm::vec2(10, 0),
        //glm::vec2(-10, 20),
        //glm::vec2(10, 20),
        //glm::vec2(0, 20),
        glm::vec2(0, 0),
        glm::vec2(-10, 10),
        glm::vec2(10, 10),
        glm::vec2(0, 10),
        glm::vec2(-11, 0),
        glm::vec2(11, 0)
    };

    for (size_t i=0; i<ARRAY_SIZE(pts); ++i)
    {
        pt = pts[i];

        calcBarycentric(tri[0], tri[1], tri[3], pt, bary);
        assert(glm::equalEpsilonGTX(pt, bary.x*tri[0]+bary.y*tri[1]+bary.z*tri[3], ml::EPS7));

        glm::vec3 scale = bary/glm::vec3(cp[0].z, cp[1].z, cp[3].z);
        glm::vec4 klmnI1 = (scale.x*klmn[0] + scale.y*klmn[1] + scale.z*klmn[3])/(scale.x + scale.y + scale.z);

        float f1  = klmnI1.x*klmnI1.x*klmnI1.x - klmnI1.y*klmnI1.z*klmnI1.w;
        float iw1 = scale.x+scale.y+scale.z;

        calcBarycentric(tri[1], tri[2], tri[3], pt, bary);
        assert(glm::equalEpsilonGTX(pt, bary.x*tri[1]+bary.y*tri[2]+bary.z*tri[3], ml::EPS7));
        scale = bary/glm::vec3(cp[1].z, cp[2].z, cp[3].z);
        glm::vec4 klmnI2 = (scale.x*klmn[1] + scale.y*klmn[2] + scale.z*klmn[3])/(scale.x + scale.y + scale.z);

        float f2  = klmnI2.x*klmnI2.x*klmnI2.x - klmnI2.y*klmnI2.z*klmnI2.w;
        float iw2 = scale.x+scale.y+scale.z;

        glm::vec4 sc = klmnI1/klmnI2;
        float s2s = iw2/iw1;

        assert(ml::equalE(f1*iw1*iw1*iw1, f2*iw2*iw2*iw2, ml::EPS5));
        //assert(ml::equalE(iw1, iw2));

        calcBarycentric(tri[1], tri[2], pt, tri[3], bary);
        assert(glm::equalEpsilonGTX(tri[3], bary.x*tri[1]+bary.y*tri[2]+bary.z*pt, ml::EPS7));
        scale = bary/glm::vec3(cp[1].z, cp[2].z, 1/iw2);
        glm::vec4 klmnII = (scale.x*klmn[1] + scale.y*klmn[2] + scale.z*klmnI2)/(scale.x + scale.y + scale.z);

        float f3  = klmnII.x*klmnII.x*klmnII.x - klmnII.y*klmnII.z*klmnII.w;
        float iw3 = scale.x+scale.y+scale.z;

        assert(ml::equalE(cp[3].z, 1/iw3, ml::EPS5));
        assert(glm::equalEpsilonGTX(klmn[3], klmnII, ml::EPS7));
    }

    glm::vec2 n1 = glm::normalize(perpendicularCCW(tri[1]-tri[0]));
    glm::vec2 n2 = glm::normalize(perpendicularCCW(tri[2]-tri[1]));
    glm::vec2 n3 = glm::normalize(perpendicularCCW(tri[3]-tri[2]));

    glm::vec2 o1 = 10.0f*n1;
    glm::vec2 o2 = 10.0f*calcOffsetN(n1, n2);
    glm::vec2 o3 = 10.0f*calcOffsetN(n2, n3);
    glm::vec2 o4 = 10.0f*n3;

    size_t base = distTransGeom.vertices.size();
    distTransGeom.vertices.resize(base+6);

    float iw;
    float fv;
    glm::vec4 klmnN;

    pt = tri[0]+o1;//glm::vec2(-40, 0);//tri[0]+o1;
    calcBarycentric(tri[0], tri[1], tri[2], pt, bary);
    assert(glm::equalEpsilonGTX(pt, bary.x*tri[0]+bary.y*tri[1]+bary.z*tri[3], ml::EPS7));
    klmnN = distTransGeom.vertices[base+0].klmn = perspectiveInterpolate(bary, glm::vec3(cp[0].z, cp[1].z, cp[2].z), klmn[0], klmn[1], klmn[2], iw);
    //distTransGeom.vertices[base+0].klmn.x = -distTransGeom.vertices[base+0].klmn.x;
    //distTransGeom.vertices[base+0].klmn.z = -distTransGeom.vertices[base+0].klmn.z;
    fv  = klmnN.x*klmnN.x*klmnN.x - klmnN.y*klmnN.z*klmnN.w;
    distTransGeom.vertices[base+0].pos = glm::vec4(pt, 0, 1)/iw;

    pt = tri[1]+o2;//glm::vec2(-40, 100);//tri[1]+o2;
    calcBarycentric(tri[0], tri[1], tri[2], pt, bary);
    klmnN = distTransGeom.vertices[base+1].klmn = perspectiveInterpolate(bary, glm::vec3(cp[0].z, cp[1].z, cp[2].z), klmn[0], klmn[1], klmn[2], iw);
    fv  = klmnN.x*klmnN.x*klmnN.x - klmnN.y*klmnN.z*klmnN.w;
    distTransGeom.vertices[base+1].pos = glm::vec4(pt, 0, 1)/iw;

    pt = tri[2]+o3;//glm::vec2(40, 100);//tri[2]+o3;
    calcBarycentric(tri[0], tri[1], tri[2], pt, bary);
    klmnN = distTransGeom.vertices[base+2].klmn = perspectiveInterpolate(bary, glm::vec3(cp[0].z, cp[1].z, cp[2].z), klmn[0], klmn[1], klmn[2], iw);
    fv  = klmnN.x*klmnN.x*klmnN.x - klmnN.y*klmnN.z*klmnN.w;
    distTransGeom.vertices[base+2].pos = glm::vec4(pt, 0, 1)/iw;

    pt = tri[3]+o4;
    calcBarycentric(tri[0], tri[1], tri[2], pt, bary);
    klmnN = distTransGeom.vertices[base+3].klmn = perspectiveInterpolate(bary, glm::vec3(cp[0].z, cp[1].z, cp[2].z), klmn[0], klmn[1], klmn[2], iw);
    fv  = klmnN.x*klmnN.x*klmnN.x - klmnN.y*klmnN.z*klmnN.w;
    distTransGeom.vertices[base+3].pos = glm::vec4(pt, 0, 1)/iw;

    pt = tri[3]-o4;
    calcBarycentric(tri[0], tri[1], tri[2], pt, bary);
    klmnN = distTransGeom.vertices[base+4].klmn = perspectiveInterpolate(bary, glm::vec3(cp[0].z, cp[1].z, cp[2].z), klmn[0], klmn[1], klmn[2], iw);
    fv  = klmnN.x*klmnN.x*klmnN.x - klmnN.y*klmnN.z*klmnN.w;
    distTransGeom.vertices[base+4].pos = glm::vec4(pt, 0, 1)/iw;

    pt = tri[0]-o1;//glm::vec2(40, 0);//tri[0]-o1;
    calcBarycentric(tri[0], tri[1], tri[2], pt, bary);
    klmnN = distTransGeom.vertices[base+5].klmn = perspectiveInterpolate(bary, glm::vec3(cp[0].z, cp[1].z, cp[2].z), klmn[0], klmn[1], klmn[2], iw);
    fv  = klmnN.x*klmnN.x*klmnN.x - klmnN.y*klmnN.z*klmnN.w;
    distTransGeom.vertices[base+5].pos = glm::vec4(pt, 0, 1)/iw;

    distTransGeom.indices.pushBack(base+0);
    distTransGeom.indices.pushBack(base+1);
    distTransGeom.indices.pushBack(base+5);

    distTransGeom.indices.pushBack(base+1);
    distTransGeom.indices.pushBack(base+2);
    distTransGeom.indices.pushBack(base+5);

    distTransGeom.indices.pushBack(base+2);
    distTransGeom.indices.pushBack(base+3);
    distTransGeom.indices.pushBack(base+4);

    distTransGeom.indices.pushBack(base+2);
    distTransGeom.indices.pushBack(base+4);
    distTransGeom.indices.pushBack(base+5);
}

GLuint createRenderbuffer(GLenum type, GLuint samples, GLsizei width, GLsizei height)
{
    GLuint rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, type, width, height);
    //glRenderbufferStorage(GL_RENDERBUFFER, type, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    return rbo;
}

//Use framebuffers just like layouts using shared components
GLuint createFramebuffer(GLuint colorRenderbuffer, GLuint depthRenderbuffer)
{
    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorRenderbuffer);
    glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);
    //glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthRenderbuffer);

    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    return fbo;
}

void meshAddBezier3(vg::geometry_t* geom, uint16_t prevIdx, uint16_t curIdx, const glm::vec2& cp0, const glm::vec2&  cp1, const glm::vec2& cp2, const glm::vec2& cp3)
{
    glm::vec2   ctrlPt[4*2] = {cp0, cp1, cp2, cp3};
    glm::vec3   klm[4*2];
    int         count;
    float       subdPts[2];

    bezier3MakeImplicit(ctrlPt, klm, count, subdPts);

    if (count>1)
    {
        orderAscend(subdPts[0], subdPts[1]);
    }

    for(int i=0; i<count; ++i)
    {
        float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

        cubic::subdivide(ctrlPt, t, ctrlPt+4, ctrlPt);
        cubic::subdivide(klm, t, klm+4, klm);

        cubic::correctOrient(klm+4);
        vg::geomAddB3Vertices(geom, (ml::vec2*)ctrlPt+4, (ml::vec3*)klm+4);

        //Carefully, we changed places of subdivided curves,
        //that's why suitable points are 0 and 7
        uint16_t idx = vg::geomAddVertex(geom, *(ml::vec2*)&ctrlPt[0]);
        vg::geomAddTri(geom, prevIdx, idx, curIdx);
        prevIdx = idx;
    }

    cubic::correctOrient(klm);
    vg::geomAddB3Vertices(geom, (ml::vec2*)ctrlPt, (ml::vec3*)klm);
}

static const float hw = 0.1f;

//TODOD: Move subdivision logic into main func, and stroke creation logic for single curve retain here 
void meshStrokeSubdivideBezier3(vg::geometry_t* geom, uint16_t prevIdx, uint16_t curIdx, const glm::vec2& cp0, const glm::vec2&  cp1, const glm::vec2& cp2, const glm::vec2& cp3)
{
    glm::vec2 cps[8] = {cp0, cp1, cp2, cp3};
    float l0 = glm::length(cp3-cp0), l1=glm::length(cp1-cp0), l2=glm::length(cp2-cp1), l3=glm::length(cp3-cp2), l=l1+l2+l3;
    int maxCount = 4;
    float k = glm::clamp(1.0f-l0/l, 0.0f, 1.0f);
    int count = (int)floor(4*k+0.5);//add dependency on half width!!!!
    for(int i=0; i<count-1; ++i)
    {
        float t = 1.0f/(count-(float)i);//add dependency on l1, l2, l3

        cubic::subdivide(cps, t, cps+4, cps);
        glm::vec2 n0 = calcOffset(cps[4], cps[5]);
        glm::vec2 n1 = calcOffset(cps[4], cps[5], cps[6]);
        glm::vec2 n2 = calcOffset(cps[5], cps[6], cps[7]);
        glm::vec2 n3 = calcOffset(cps[6], cps[7]);
        glm::vec2 p0pos = cps[4]+n0*hw;
        glm::vec2 p1pos = cps[5]+n1*hw;
        glm::vec2 p2pos = cps[6]+n2*hw;
        glm::vec2 p3pos = cps[7]+n3*hw;
        glm::vec2 p0neg = cps[4]-n0*hw;
        glm::vec2 p1neg = cps[5]-n1*hw;
        glm::vec2 p2neg = cps[6]-n2*hw;
        glm::vec2 p3neg = cps[7]-n3*hw;
        uint16_t i0 = vg::geomAddVertex(geom, *(ml::vec2*)&p0pos);
        uint16_t i1 = vg::geomAddVertex(geom, *(ml::vec2*)&p0neg);
        uint16_t i2 = vg::geomAddVertex(geom, *(ml::vec2*)&p3pos);
        uint16_t i3 = vg::geomAddVertex(geom, *(ml::vec2*)&p3neg);
        meshAddBezier3(geom, i0, i2, p0pos, p1pos, p2pos, p3pos);
        meshAddBezier3(geom, i3, i1, p3neg, p2neg, p1neg, p0neg);
        vg::geomAddTri(geom, i0, i2, i1);
        vg::geomAddTri(geom, i1, i2, i3);
    }
    glm::vec2 n0 = calcOffset(cps[0], cps[1]);
    glm::vec2 n1 = calcOffset(cps[0], cps[1], cps[2]);
    glm::vec2 n2 = calcOffset(cps[1], cps[2], cps[3]);
    glm::vec2 n3 = calcOffset(cps[2], cps[3]);
    glm::vec2 p0pos = cps[0]+n0*hw;
    glm::vec2 p1pos = cps[1]+n1*hw;
    glm::vec2 p2pos = cps[2]+n2*hw;
    glm::vec2 p3pos = cps[3]+n3*hw;
    glm::vec2 p0neg = cps[0]-n0*hw;
    glm::vec2 p1neg = cps[1]-n1*hw;
    glm::vec2 p2neg = cps[2]-n2*hw;
    glm::vec2 p3neg = cps[3]-n3*hw;
    uint16_t i0 = vg::geomAddVertex(geom, *(ml::vec2*)&p0pos);
    uint16_t i1 = vg::geomAddVertex(geom, *(ml::vec2*)&p0neg);
    uint16_t i2 = vg::geomAddVertex(geom, *(ml::vec2*)&p3pos);
    uint16_t i3 = vg::geomAddVertex(geom, *(ml::vec2*)&p3neg);
    meshAddBezier3(geom, i0, i2, p0pos, p1pos, p2pos, p3pos);
    meshAddBezier3(geom, i3, i1, p3neg, p2neg, p1neg, p0neg);
    vg::geomAddTri(geom, i0, i2, i1);
    vg::geomAddTri(geom, i1, i2, i3);
}

void meshStrokeBezier3(vg::geometry_t* geom, uint16_t prevIdx, uint16_t curIdx, const glm::vec2& cp0, const glm::vec2&  cp1, const glm::vec2& cp2, const glm::vec2& cp3)
{
    glm::vec3   klmT[4*2];
    int         count, countT;
    float       subdPts[2], subdPtsT[2];

    glm::vec2 cps[8] = {cp0, cp1, cp2, cp3};
    bezier3MakeImplicit(cps, klmT, countT, subdPtsT);
    bezier3SpecialPts(cps, count, subdPts);
    assert(count==countT);
    if (count>0) assert(ml::equalE(subdPts[0], subdPtsT[0]));
    if (count>1) assert(ml::equalE(subdPts[1], subdPtsT[1]));

    if (count>1)
    {
        orderAscend(subdPts[0], subdPts[1]);
    }

    for(int i=0; i<count; ++i)
    {
        float t = i==0?subdPts[i]:(subdPts[i]-subdPts[i-1])/(1-subdPts[i-1]);

        cubic::subdivide(cps, t, cps+4, cps);
        meshStrokeSubdivideBezier3(geom, prevIdx, curIdx, cps[4], cps[5], cps[6], cps[7]);
        glm::vec2 n = calcOffset(cps[4], cps[5]);
    }

    meshStrokeSubdivideBezier3(geom, prevIdx, curIdx, cps[0], cps[1], cps[2], cps[3]);
}

namespace app
{
    GLuint colorRB, depthRB, fbo;
    Geometry<CubicVertex>   mRasterCubic;
    Geometry<glm::vec2>     mRationalCubic, mTri;
    vg::Path                testPath, testPathOff;

    bool doMove = false;
    float offsetX;
    float offsetY;
    float zoom = 10.0f;

    void tessellateCubic();

    void init()
    {
        offsetX = gfx::width  / 2.0f;
        offsetY = gfx::height / 2.0f;

        initVGExp();

        //addDistanceTransformCubic(controlPts, rcubic);
        addCubic(controlPts, rcubic, rtri);
        glm::vec3 cpt[3] = 
        {
            glm::vec3(-20, 0, 1),
            glm::vec3(0, 20, 1),
            glm::vec3(20, 5, 1)
        };

        //addAATriangle(cpt, rcubic);
        //testRCubic();
        tessellateCubic();

        Array<glm::vec2> cp;
        cp.assign(controlPts2, controlPts2+4);
        addCubic(mRasterCubic, mTri, cp);

        glClearStencil(0x80);
        colorRB = createRenderbuffer(GL_RGBA8, 8, 800, 600);
        depthRB = createRenderbuffer(GL_DEPTH24_STENCIL8, 8, 800, 600);
        fbo = createFramebuffer(colorRB, depthRB);

        glm::vec2 cps[8] = {
            glm::vec2(-20.0f,  0.0f),
            glm::vec2( 30.0f, 40.0f),
            glm::vec2(-30.0f, 40.0f),
            glm::vec2( 20.0f,  0.0f),
        };

        const size_t maxIndices    = 100 * 9;
        const size_t maxVertices   = 100 * 5;
        const size_t maxB3Vertices = 100 * 10;

        vg::geometry_t  geomPath = {
            0, 0, 0,
            (uint16_t*)core::thread_stack_alloc(sizeof(uint16_t)*maxIndices),
            (ml::vec2*)core::thread_stack_alloc(sizeof(ml::vec2)*maxVertices),
            (vg::B3Vertex*)core::thread_stack_alloc(sizeof(vg::B3Vertex)*maxB3Vertices)
        };

        vg::geometry_t  geomPathOff = {
            0, 0, 0,
            (uint16_t*)core::thread_stack_alloc(sizeof(uint16_t)*maxIndices),
            (ml::vec2*)core::thread_stack_alloc(sizeof(ml::vec2)*maxVertices),
            (vg::B3Vertex*)core::thread_stack_alloc(sizeof(vg::B3Vertex)*maxB3Vertices)
        };

        uint16_t  prevIdx,  curIdx;
        uint16_t  prevIdx2, curIdx2;

        prevIdx = 0;
        curIdx = vg::geomAddVertex(&geomPath, *(ml::vec2*)&cps[0]);

        prevIdx2 = vg::geomAddVertex(&geomPathOff, *(ml::vec2*)&cps[0]);
        curIdx2  = vg::geomAddVertex(&geomPathOff, *(ml::vec2*)&cps[3]);

        meshAddBezier3   (&geomPath,    prevIdx,  curIdx,  cps[0], cps[1], cps[2], cps[3]);
        meshStrokeBezier3(&geomPathOff, prevIdx2, curIdx2, cps[0], cps[1], cps[2], cps[3]);

        testPath    = vg::geomToPath(&geomPath);
        testPathOff = vg::geomToPath(&geomPathOff);

        core::thread_stack_reset(geomPath.indices);
    }

    void fini()
    {
        vg::destroyPath(testPath);
        vg::destroyPath(testPathOff);

        glDeleteFramebuffers(1, &fbo);
        glDeleteRenderbuffers(1, &depthRB);
        glDeleteRenderbuffers(1, &colorRB);

        terminateVGExp();
    }

    void tessellateCubic()
    {
        int numPts = 1300;
        float step = 1.0f/(numPts-1);

        mRationalCubic.vertices.resize(numPts);
        mRationalCubic.indices.resize((numPts-2)*3);
        int i=0;
        for (float t=0; t<=1; t+=step, ++i)
        {
            float it = 1-t;
            glm::vec3	pp = it*it*it*controlPts[0]+3*it*it*t*controlPts[1]+3*it*t*t*controlPts[2]+t*t*t*controlPts[3];
            mRationalCubic.vertices[i] = glm::vec2(pp)/pp.z;
            if (i>1)
            {
                mRationalCubic.indices[(i-2)*3+0] = 0;
                mRationalCubic.indices[(i-2)*3+1] = i-1;
                mRationalCubic.indices[(i-2)*3+2] = i;
            }
        }
    }

    void render()
    {
        ui::processZoomAndPan(zoom, offsetX, offsetY, doMove);

        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        glTranslatef(offsetX, offsetY, 0);
        glScalef(zoom, zoom, 1);

        vg::drawPath  (testPath,    0xFF, 0xFF, 0xFF, 0xFF);
        vg::drawPathNZ(testPathOff, 0xFF, 0x00, 0x00, 0xFF);
        glColor3f(1, 0, 0);
        glBegin(GL_LINE_STRIP);
        glVertex2f(controlPts[0].x/controlPts[0].z, controlPts[0].y/controlPts[0].z);
        glVertex2f(controlPts[1].x/controlPts[1].z, controlPts[1].y/controlPts[1].z);
        glVertex2f(controlPts[2].x/controlPts[2].z, controlPts[2].y/controlPts[2].z);
        glVertex2f(controlPts[3].x/controlPts[3].z, controlPts[3].y/controlPts[3].z);
        glEnd();

        glPopMatrix();
        return;

        float vp[4];
        glGetFloatv(GL_VIEWPORT, vp);

        glPushMatrix();
        if (bool alphaAA = false)
        {
            glDisable(GL_STENCIL_TEST);

            glMatrixMode(GL_MODELVIEW);
            glTranslatef(offsetX, offsetY, 0);
            glScalef(zoom, zoom, 1);

            glPushMatrix();
            glTranslatef(-40, -40, 0);
            glScalef(80, 80, 1);
            clearAlpha();
            glPopMatrix();

            rasterizeEvenOddAA(rcubic);
            rasterizeEvenOddAA(rtri);

            glUseProgram(0);
            glPushMatrix();
            glTranslatef(-40, -40, 0);
            glScalef(80, 80, 1);
            drawAlphaQuad();
            glPopMatrix();

            glColor3f(1, 0, 0);
            glBegin(GL_LINE_STRIP);
            glVertex2f(controlPts[0].x/controlPts[0].z, controlPts[0].y/controlPts[0].z);
            glVertex2f(controlPts[1].x/controlPts[1].z, controlPts[1].y/controlPts[1].z);
            glVertex2f(controlPts[2].x/controlPts[2].z, controlPts[2].y/controlPts[2].z);
            glVertex2f(controlPts[3].x/controlPts[3].z, controlPts[3].y/controlPts[3].z);
            glEnd();
        }
        else if (bool multisample = true)
        {
            //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, fbo);

            //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glEnable(GL_STENCIL_TEST);
            glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);

            glMatrixMode(GL_MODELVIEW);
            glTranslatef(offsetX, offsetY, 0);
            glScalef(zoom, zoom, 1);

            glPushMatrix();
            glTranslatef(-40, -40, 0);
            glScalef(80, 80, 1);
            clearStencil();
            glPopMatrix();

            glPushMatrix();
            //glScalef(-1, 1, 1);
            drawRCubicA2C(rcubic);
            rasterizeEvenOdd(rtri);
            glPopMatrix();

            glUseProgram(0);

            glPushMatrix();
            glTranslatef(-40, -40, 0);
            glScalef(80, 80, 1);
            drawQuad();
            glPopMatrix();

            glDisable(GL_STENCIL_TEST);

            glColor3f(1, 0, 0);
            glBegin(GL_LINE_STRIP);
            glVertex2f(controlPts[0].x/controlPts[0].z, controlPts[0].y/controlPts[0].z);
            glVertex2f(controlPts[1].x/controlPts[1].z, controlPts[1].y/controlPts[1].z);
            glVertex2f(controlPts[2].x/controlPts[2].z, controlPts[2].y/controlPts[2].z);
            glVertex2f(controlPts[3].x/controlPts[3].z, controlPts[3].y/controlPts[3].z);
            glEnd();

            glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

            //glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            //glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);

            //glBlitFramebuffer(
            //		0, 0, 800, 600,
            //		0, 0, 800, 600,
            //		//if enable depth and stencil resolve - label stops to render.
            //		GL_COLOR_BUFFER_BIT/*|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT*/,
            //		GL_NEAREST);

            //glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
        }
        else
        {
            glEnable(GL_STENCIL_TEST);

            glMatrixMode(GL_MODELVIEW);
            glPushMatrix();
            glTranslatef(offsetX, offsetY, 0);
            glScalef(zoom, zoom, 1);

            glPushMatrix();
            glTranslatef(-40, -40, 0);
            glScalef(80, 80, 1);
            clearStencil();
            glPopMatrix();
            //rasterizeEvenOdd(mRationalCubic);
            //rasterizeEvenOdd(mRasterCubic);
            //rasterizeEvenOdd(rcubic);
            //rasterizeEvenOdd(rtri);
            glPushMatrix();
            //glScalef(-1, 1, 1);
            //drawRCubicAA(rcubic);
            drawRCubicDTAA(rcubic);
            //drawCubicAA(mRasterCubic);
            glPopMatrix();
            //rasterizeEvenOdd(mTri);
            glUseProgram(0);
            glPushMatrix();
            glTranslatef(-40, -40, 0);
            glScalef(80, 80, 1);
            drawQuad();
            glPopMatrix();
            glDisable(GL_STENCIL_TEST);
            glColor3f(1, 0, 0);
            glBegin(GL_LINE_STRIP);
            glVertex2f(controlPts[0].x/controlPts[0].z, controlPts[0].y/controlPts[0].z);
            glVertex2f(controlPts[1].x/controlPts[1].z, controlPts[1].y/controlPts[1].z);
            glVertex2f(controlPts[2].x/controlPts[2].z, controlPts[2].y/controlPts[2].z);
            glVertex2f(controlPts[3].x/controlPts[3].z, controlPts[3].y/controlPts[3].z);
            glEnd();
        }
        glPopMatrix();

        glColor3f(1.0f, 1.0f, 1.0f);
        char zoomStr[256];
        _snprintf(zoomStr, 256, "Zoom level %f", zoom);
        vg::drawString(vg::defaultFont, 300, 10, zoomStr, strlen(zoomStr));
    }

    void update(float){}
    void resize(int, int) {}
    void recompilePrograms() {}
}
