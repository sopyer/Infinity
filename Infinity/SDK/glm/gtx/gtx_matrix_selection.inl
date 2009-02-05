//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2005-12-21
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_selection.inl
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_selection_inl__
#define __gtx_matrix_selection_inl__

#include "gtx_matrix_selection.h"
#if(GLM_INCLUDED(GLM_GTX_matrix_selection, GLM_EXTENSION_GTX, GLM_GTX_matrix_selection_required))

namespace glm
{
    template <typename T>
    inline _xvec2<T> rowGTX(const _xmat2<T>& m, int index)
    {
        return glm::_xvec2<T>(m[0][index], m[1][index]);
    }

    template <typename T>
    inline _xvec3<T> rowGTX(const _xmat3<T>& m, int index)
    {
        return glm::_xvec3<T>(m[0][index], m[1][index], m[2][index]);
    }

    template <typename T>
    inline _xvec4<T> rowGTX(const _xmat4<T>& m, int index)
    {
        return glm::_xvec4<T>(m[0][index], m[1][index], m[2][index], m[3][index]);
    }

    template <typename T>
    inline _xvec2<T> columnGTX(const _xmat2<T>& m, int index)
    {
        return m[index];
    }

    template <typename T>
    inline _xvec3<T> columnGTX(const _xmat3<T>& m, int index)
    {
        return m[index];
    }

    template <typename T>
    inline _xvec4<T> columnGTX(const _xmat4<T>& m, int index)
    {
        return m[index];
    }
}

#endif//included

#endif//__gtx_matrix_selection_inl__
