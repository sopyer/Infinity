//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _xref2.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xref2_h__
#define __xref2_h__

namespace glm
{
    template <typename T>
    struct _xref2
    {
        _xref2(T& x, T& y);
        _xref2<T>& operator= (const _xref2<T>& r);
        _xref2<T>& operator+=(const _xref2<T>& r);
	    _xref2<T>& operator-=(const _xref2<T>& r);
	    _xref2<T>& operator*=(const _xref2<T>& r);
	    _xref2<T>& operator/=(const _xref2<T>& r);
        _xref2<T>& operator= (const _xvec2<T>& v);
        _xref2<T>& operator+=(const _xvec2<T>& v);
	    _xref2<T>& operator-=(const _xvec2<T>& v);
	    _xref2<T>& operator*=(const _xvec2<T>& v);
	    _xref2<T>& operator/=(const _xvec2<T>& v);
        T& x;
        T& y;
    };
}

#endif//__xref2_h__
