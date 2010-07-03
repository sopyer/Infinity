//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_selection.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_selection_h__
#define __gtx_matrix_selection_h__

#if (GLM_GTX_matrix_selection == GLM_EXTENSION_DISABLE && defined(GLM_GTX_matrix_selection_required))
#error GLM_GTX_matrix_selection is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_matrix_selection, GLM_EXTENSION_GTX, GLM_GTX_matrix_selection_required))

//#define GLM_GTX_matrix_selection_supported 

namespace glm
{
    template <typename T> _xvec2<T> rowGTX(const _xmat2<T>& m, int index); //!< \brief Returns a 2 components vector that contains the row of the matrix m witch values is the ones of the row index (from GLM_GTX_matrix_selection extension)
    template <typename T> _xvec3<T> rowGTX(const _xmat3<T>& m, int index); //!< \brief Returns a 3 components vector that contains the row of the matrix m witch values is the ones of the row index (from GLM_GTX_matrix_selection extension)
    template <typename T> _xvec4<T> rowGTX(const _xmat4<T>& m, int index); //!< \brief Returns a 4 components vector that contains the row of the matrix m witch values is the ones of the row index (from GLM_GTX_matrix_selection extension)
    template <typename T> _xvec2<T> columnGTX(const _xmat2<T>& m, int index); //!< \brief Returns a 2 components vector that contains the row of the matrix m witch values is the ones of the column index (from GLM_GTX_matrix_selection extension)
    template <typename T> _xvec3<T> columnGTX(const _xmat3<T>& m, int index); //!< \brief Returns a 3 components vector that contains the row of the matrix m witch values is the ones of the column index (from GLM_GTX_matrix_selection extension)
    template <typename T> _xvec4<T> columnGTX(const _xmat4<T>& m, int index); //!< \brief Returns a 4 components vector that contains the row of the matrix m witch values is the ones of the column index (from GLM_GTX_matrix_selection extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xvec2<T> row(const _xmat2<T>& m, int index){return rowGTX(m, index);}
    template <typename T> _xvec3<T> row(const _xmat3<T>& m, int index){return rowGTX(m, index);}
    template <typename T> _xvec4<T> row(const _xmat4<T>& m, int index){return rowGTX(m, index);}
    template <typename T> _xvec2<T> column(const _xmat2<T>& m, int index){return columnGTX(m, index);}
    template <typename T> _xvec3<T> column(const _xmat3<T>& m, int index){return columnGTX(m, index);}
    template <typename T> _xvec4<T> column(const _xmat4<T>& m, int index){return columnGTX(m, index);}

#endif
}

#endif//included

#endif//__gtx_matrix_selection_h__
