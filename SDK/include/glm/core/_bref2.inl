//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _bref2.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bref2_inl__
#define __bref2_inl__

#include "./_bref2.h"

namespace glm
{
    inline _bref2::_bref2(bool& x, bool& y) :
        x(x),
        y(y)
    {}

    inline _bref2& _bref2::operator=(const _bref2& r)
    {
        x = r.x;
        y = r.y;
        return *this;
    }

    inline _bref2& _bref2::operator=(const _bvec2& v)
    {
        x = v.x;
        y = v.y;
        return *this;
    }
}

#endif//__bref2_inl__
