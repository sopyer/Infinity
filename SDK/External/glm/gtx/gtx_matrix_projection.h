//////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2006 G-Truc Creation (www.g-truc.net)
//////////////////////////////////////////////////////////////////////////////////
// Author  : Christophe [Groove] Riccio (contact [at] g-truc [point] net)
// Created : 2005-12-21
// Updated : 2006-11-13
// Licence : This source is under GNU LGPL licence
// File    : gtx_matrix_projection.h
//////////////////////////////////////////////////////////////////////////////////
// Dependency:
// - GLM core
//////////////////////////////////////////////////////////////////////////////////

#ifndef __gtx_matrix_projection_h__
#define __gtx_matrix_projection_h__

#if (GLM_GTX_matrix_projection == GLM_EXTENSION_DISABLE && defined(GLM_GTX_matrix_projection_required))
#error GLM_GTX_matrix_projection is disable but required
#endif

// Dependency:
#include "../glm.h"

#if(GLM_INCLUDED(GLM_GTX_matrix_projection, GLM_EXTENSION_GTX, GLM_GTX_matrix_projection_required))

//#define GLM_GTX_matrix_projection_supported 

namespace glm
{
    template <typename T> _xmat4<T> ortho2DGTX(T left, T right, T bottom, T top); //!< \brief Creates a matrix for projecting two-dimensional coordinates onto the screen (from GLM_GTX_matrix_projection extension)
    template <typename T> _xmat4<T> ortho3DGTX(T left, T right, T bottom, T top, T zNear, T zFar); //!< \brief Creates a matrix for an orthographic parallel viewing volume (from GLM_GTX_matrix_projection extension)
    template <typename T> _xmat4<T> perspectiveGTX(T fovy, T aspect, T zNear, T zFar); //!< \brief Creates a matrix for a symmetric perspective-view frustum (from GLM_GTX_matrix_projection extension)
    template <typename T> _xmat4<T> infinitePerspectiveGTX(T fovy, T aspect, T zNear);  //!< \brief Creates a matrix for a symmetric perspective-view frustum with far plane at infinite (from GLM_GTX_matrix_projection extension)
    template <typename T> _xmat4<T> tweakedInfinitePerspectiveGTX(T fovy, T aspect, T zNear); //!< \brief Creates a matrix for a symmetric perspective-view frustum with far plane at infinite for graphics hardware that doesn't support depth clamping (from GLM_GTX_matrix_projection extension)
    template <typename T, typename U> _xvec3<T> projectGTX(const _xvec3<T>& obj, const _xmat4<T>& model, const _xmat4<T>& proj, const _xvec4<U>& view); //!< \brief Map the specified object coordinates (obj.x, obj.y, obj.z) into window coordinates (from GLM_GTX_matrix_projection extension)
    template <typename T, typename U> _xvec3<T> unProjectGTX(const _xvec3<T>& win, const _xmat4<T>& model, const _xmat4<T>& proj, const _xvec4<U>& view); //!< \brief Map the specified window coordinates (win.x, win.y, win.z) into object coordinates (from GLM_GTX_matrix_projection extension)

#if (GLM_CONFORMANCE == GLM_CONFORMANCE_FRIENDLY)

    template <typename T> _xmat4<T> ortho2D(T left, T right, T bottom, T top){return ortho2DGTX(left, right, bottom, top);}
    template <typename T> _xmat4<T> ortho3D(T left, T right, T bottom, T top, T zNear, T zFar){return ortho2DGTX(left, right, bottom, top, zNear, zFar);}
    template <typename T> _xmat4<T> perspective(T fovy, T aspect, T zNear, T zFar){return perspectiveGTX(fovy, aspect, zNear, zFar);}
    template <typename T> _xmat4<T> infinitePerspective(T fovy, T aspect, T zNear){return infinitePerspectiveGTX(fovy, aspect, zNear);}
    template <typename T> _xmat4<T> tweakedInfinitePerspective(T fovy, T aspect, T zNear){return tweakedInfinitePerspectiveGTX(fovy, aspect, zNear);}
    template <typename T, typename U> _xvec3<T> project(const _xvec3<T>& obj, const _xmat4<T>& model, const _xmat4<T>& proj, const _xvec4<U>& view){return project(obj, model, proj, view);}
    template <typename T, typename U> _xvec3<T> unProject(const _xvec3<T>& win, const _xmat4<T>& model, const _xmat4<T>& proj, const _xvec4<U>& view){return unProject(win, model, proj, view);}

#endif
}

#endif//included

#endif//__gtx_matrix_projection_h__
