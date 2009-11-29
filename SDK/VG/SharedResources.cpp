#include <gl\glee.h>
#include <cassert>
#include "SharedResources.h"

namespace impl
{
	namespace shared
	{
		GLuint	prgMaskQuad = 0;
		GLuint	prgMaskCubic = 0;
		GLuint	prgMaskArc = 0;	
		GLuint	prgFillColor = 0;
		GLuint	prgFillPattern = 0;
		GLuint	prgMaskStrokeSeg = 0;
		GLuint	prgStrokeMaskQuad = 0;
		GLuint	prgStrokeMaskCubic = 0;
		GLuint	prgStrokeMaskArc = 0;
		
		GLint	attrOffsetCubic = -1;
		GLint	attrOffsetArc = -1;
		GLint	locOffsetAttribQuad = -1;
		GLint	locOffsetAttrib = -1;

		namespace
		{
			size_t refCount = 0;

			const char* const sourceArcFragSh = 
				"void main(void)											\n"
				"{															\n"
				"	vec2 uv = gl_TexCoord[0].st;							\n"
				"															\n"
				"	if( (uv.s*uv.s + uv.t*uv.t)>=1.0 )						\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n";

			const char* const sourceQuadFragSh = 
				"void main(void)											\n"
				"{															\n"
				"	vec2 uv = gl_TexCoord[0].st;							\n"
				"															\n"
				"	if( (pow(uv.s, 2.0)-uv.t)>=0.0 )						\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n";

			const char* const sourceCubicFragSh = 
				"void main(void)											\n"
				"{															\n"
				"	vec3 uv = gl_TexCoord[0].stp;							\n"
				"															\n"
				"	if( (uv.s*uv.s*uv.s - uv.t*uv.p)>=0.0 )					\n"
				"		discard;											\n"
				"															\n"
				"	gl_FragColor = vec4(1.0);								\n"
				"}															\n";

			const char* const sourceColorFillFragSh =
				"uniform vec4 uFillColor;									\n"
				"															\n"
				"void main()												\n"
				"{															\n"
				"	gl_FragColor = uFillColor;								\n"
				"}															\n";

			const char* const sourceFillVertSh =
				"uniform vec2 uImageDim;									\n"
				"															\n"
				"void main()												\n"
				"{															\n"
				"	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;	\n"
				"	gl_TexCoord[0] = vec4(gl_Vertex.xy/uImageDim, 0, 0);	\n"
				"}															\n";

			const char* const sourcePatternFillFragSh =
				"uniform sampler2D uPattern;								\n"
				"															\n"
				"void main()												\n"
				"{															\n"
				"	gl_FragColor = tex2D(uPattern, gl_TexCoord[0].st);		\n"
				"}															\n";

			const char* const sourceStrokeVertSh =
				"attribute vec2 aOffset;									\n"
				"															\n"
				"void main()												\n"
				"{															\n"
				"	vec4 pos = gl_Vertex;									\n"
				"	pos.xy += 10*aOffset;									\n"
				"	gl_Position = gl_ModelViewProjectionMatrix*pos;			\n"
				"	gl_TexCoord[0] = gl_MultiTexCoord0;						\n"
				"}															\n";

			void CompileShader(GLuint shader, const char* source)
			{
				GLint len = (GLint)strlen(source);
				glShaderSource(shader, 1, (const GLchar **)&source, &len);
				glCompileShader(shader); 
			}
		}


		void Acquire()
		{
			assert(refCount>=0);
			if (refCount==0)
			{
				GLuint	vsFill;
				GLuint	fsColor;
				GLuint	fsPattern;
				GLuint	fsArc;
				GLuint	fsCubic;
				GLuint	fsQuad;
				GLuint	vsStrokeSeg;

				prgMaskArc = glCreateProgram();
				fsArc = glCreateShader(GL_FRAGMENT_SHADER);
				CompileShader(fsArc, sourceArcFragSh);
				glAttachShader(prgMaskArc, fsArc);
				glLinkProgram(prgMaskArc);

				prgMaskCubic = glCreateProgram();
				fsCubic = glCreateShader(GL_FRAGMENT_SHADER);
				CompileShader(fsCubic, sourceCubicFragSh);
				glAttachShader(prgMaskCubic, fsCubic);
				glLinkProgram(prgMaskCubic);

				prgMaskQuad = glCreateProgram();
				fsQuad = glCreateShader(GL_FRAGMENT_SHADER);
				CompileShader(fsQuad, sourceQuadFragSh);
				glAttachShader(prgMaskQuad, fsQuad);
				glLinkProgram(prgMaskQuad);
				
				prgMaskStrokeSeg = glCreateProgram();
				vsStrokeSeg = glCreateShader(GL_VERTEX_SHADER);
				CompileShader(vsStrokeSeg, sourceStrokeVertSh);
				glAttachShader(prgMaskStrokeSeg, vsStrokeSeg);
				glLinkProgram(prgMaskStrokeSeg);
				locOffsetAttrib = glGetAttribLocation(prgMaskStrokeSeg, "aOffset");
				
				prgStrokeMaskQuad = glCreateProgram();
				glAttachShader(prgStrokeMaskQuad, vsStrokeSeg);
				glAttachShader(prgStrokeMaskQuad, fsQuad);
				glLinkProgram(prgStrokeMaskQuad);
				locOffsetAttribQuad = glGetAttribLocation(prgStrokeMaskQuad, "aOffset");
				
				prgStrokeMaskCubic = glCreateProgram();
				glAttachShader(prgStrokeMaskCubic, vsStrokeSeg);
				glAttachShader(prgStrokeMaskCubic, fsCubic);
				glLinkProgram(prgStrokeMaskCubic);
				attrOffsetCubic = glGetAttribLocation(prgStrokeMaskCubic, "aOffset");

				prgStrokeMaskArc = glCreateProgram();
				glAttachShader(prgStrokeMaskArc, vsStrokeSeg);
				glAttachShader(prgStrokeMaskArc, fsArc);
				glLinkProgram(prgStrokeMaskArc);
				attrOffsetArc = glGetAttribLocation(prgStrokeMaskArc, "aOffset");

				glDeleteShader(fsArc);
				glDeleteShader(fsQuad);
				glDeleteShader(fsCubic);
				glDeleteShader(vsStrokeSeg);
				
				prgFillColor = glCreateProgram();
				fsColor = glCreateShader(GL_FRAGMENT_SHADER);
				CompileShader(fsColor, sourceColorFillFragSh);
				glAttachShader(prgFillColor, fsColor);
				glLinkProgram(prgFillColor);
				glDeleteShader(fsColor);

				prgFillPattern = glCreateProgram();
				vsFill = glCreateShader(GL_VERTEX_SHADER);
				fsPattern = glCreateShader(GL_FRAGMENT_SHADER);
				CompileShader(vsFill, sourceFillVertSh);
				CompileShader(fsPattern, sourcePatternFillFragSh);
				glAttachShader(prgFillPattern, fsPattern);
				glAttachShader(prgFillPattern, vsFill);
				glLinkProgram(prgFillPattern);
				glDeleteShader(fsPattern);
				glDeleteShader(vsFill);
			}
			refCount++;
		}

		void Release()
		{
			assert(refCount>0);
			refCount--;
			if (refCount==0)
			{
				glDeleteProgram(prgMaskQuad);
				glDeleteProgram(prgMaskCubic);
				glDeleteProgram(prgMaskArc);	
				glDeleteProgram(prgFillColor);
				glDeleteProgram(prgFillPattern);
				glDeleteProgram(prgMaskStrokeSeg);
				glDeleteProgram(prgStrokeMaskQuad);
				glDeleteProgram(prgStrokeMaskCubic);
				glDeleteProgram(prgStrokeMaskArc);
			}
		}
	}
}