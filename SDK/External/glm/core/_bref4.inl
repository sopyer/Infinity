//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _bref4.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bref4_inl__
#define __bref4_inl__

#include "./_bref4.h"

namespace glm
{
    inline _bref4::_bref4(bool& x, bool& y, bool& z, bool& w) :
        x(x),
        y(y),
        z(z),
        w(w)
    {}

    inline _bref4& _bref4::operator=(const _bref4& r)
    {
        x = r.x;
        y = r.y;
        z = r.z;
        w = r.w;
        return *this;
    }

    inline _bref4& _bref4::operator=(const _bvec4& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        w = v.w;
        return *this;
    }
}

#endif//__bref4_inl__
