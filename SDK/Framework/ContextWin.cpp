#include <gl/glee.h>
#include "Context.h"

HDC hDC;

namespace ctx
{
	//struct ContextHandle
	//{
	//	HGLRC	GLContext;
	//};

	Context getCurrent()
	{
		ContextHandle ret = {wglGetCurrentContext()};
		return ret;
	}
	
	void makeCurrent(Context context)
	{
		HDC dc = wglGetCurrentDC();
		wglMakeCurrent(dc, context.GLContext);
	}

	void cloneCurrent(GLsizei count, bool share, Context ctxArray[])
	{
		HDC		dc = wglGetCurrentDC();

		for (GLsizei i=0; i<count; ++i)
		{
			ctxArray[i].GLContext = wglCreateContext(dc);
		}

		if (share)
		{
			for (GLsizei i=0; i<count; ++i)
			{
				for (GLsizei j=i+1; j<count; ++j)
				{
					wglShareLists(ctxArray[i].GLContext, ctxArray[j].GLContext);
					wglShareLists(ctxArray[j].GLContext, ctxArray[i].GLContext);
				}
			}
		}
	}

	void destroy(GLsizei count, Context ctxArray[])
	{
		for (GLsizei i=0; i<count; ++i)
		{
			wglDeleteContext(ctxArray[i].GLContext);
		}
	}

}