//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2006-05-29
// Updated : 2006-05-29
// Licence : This source is under GNU LGPL licence
// File    : _xref4.h
//////////////////////////////////////////////////////////////////////////////////

#ifndef __xref4_h__
#define __xref4_h__

namespace glm
{
    template <typename T>
    struct _xref4
    {
        _xref4(T& x, T& y, T& z, T& w);
        _xref4<T>& operator= (const _xref4<T>& r);
        _xref4<T>& operator+=(const _xref4<T>& r);
	    _xref4<T>& operator-=(const _xref4<T>& r);
	    _xref4<T>& operator*=(const _xref4<T>& r);
	    _xref4<T>& operator/=(const _xref4<T>& r);
        _xref4<T>& operator= (const _xvec4<T>& v);
        _xref4<T>& operator+=(const _xvec4<T>& v);
	    _xref4<T>& operator-=(const _xvec4<T>& v);
	    _xref4<T>& operator*=(const _xvec4<T>& v);
	    _xref4<T>& operator/=(const _xvec4<T>& v);
        T& x;
        T& y;
        T& z;
        T& w;
    };
}

#endif//__xref4_h__
