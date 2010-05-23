#ifndef __CONTEXT_H_INCLUDED__
#	define __CONTEXT_H_INCLUDED__

namespace ctx
{
	struct ContextHandle
	{
		HGLRC	GLContext;
	};

	struct ContextHandle;
	typedef ContextHandle	Context;

	Context	getCurrent();

	void	makeCurrent(Context context);
	void	cloneCurrent(GLsizei count, bool share, Context ctxArray[]);
	void	destroy(GLsizei count, Context ctxArray[]);
}

#endif