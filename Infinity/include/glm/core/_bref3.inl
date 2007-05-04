//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _bref3.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bref3_inl__
#define __bref3_inl__

#include "./_bref3.h"

namespace glm
{
    inline _bref3::_bref3(bool& x, bool& y, bool& z) :
        x(x),
        y(y),
        z(z)
    {}

    inline _bref3& _bref3::operator=(const _bref3& r)
    {
        x = r.x;
        y = r.y;
        z = r.z;
        return *this;
    }

    inline _bref3& _bref3::operator=(const _bvec3& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
        return *this;
    }
}

#endif//__bref3_inl__
