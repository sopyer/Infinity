//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _bref3.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __bref3_h__
#define __bref3_h__

namespace glm
{
    struct _bref3
    {
        _bref3(bool& x, bool& y, bool& z);
        _bref3& operator= (const _bref3& r);
        _bref3& operator= (const _bvec3& v);
        bool& x;
        bool& y;
        bool& z;
    };
}

#endif//__bref3_h__
