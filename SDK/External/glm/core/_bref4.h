//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _bref4.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bref4_h__
#define __bref4_h__

namespace glm
{
    struct _bref4
    {
        _bref4(bool& x, bool& y, bool& z, bool& w);
        _bref4& operator= (const _bref4& r);
        _bref4& operator= (const _bvec4& v);
        bool& x;
        bool& y;
        bool& z;
        bool& w;
    };
}

#endif//__bref4_h__
