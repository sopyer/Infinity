//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _bref2.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bref2_h__
#define __bref2_h__

namespace glm
{
    struct _bref2
    {
        _bref2(bool& x, bool& y);
        _bref2& operator= (const _bref2& r);
        _bref2& operator= (const _bvec2& v);
        bool& x;
        bool& y;
    };
}

#endif//__bref2_h__
