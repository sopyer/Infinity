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
		
		GLint	locOffsetAttribQuad = -1;
		GLint	locOffsetAttrib = -1;

		namespace
		{
			size_t refCount = 0;

			const char* const sourceArcFragSh = 
				"void main(void)											"
				"{															"
				"	vec2 uv = gl_TexCoord[0].st;							"
				"															"
				"	if( (uv.s*uv.s + uv.t*uv.t)>=1.0 )						"
				"		discard;											"
				"															"
				"	gl_FragColor = vec4(1.0);								"
				"}															";

			const char* const sourceQuadFragSh = 
				"void main(void)											"
				"{															"
				"	vec2 uv = gl_TexCoord[0].st;							"
				"															"
				"	if( (pow(uv.s, 2.0)-uv.t)>=0.0 )						"
				"		discard;											"
				"															"
				"	gl_FragColor = vec4(1.0);								"
				"}															";

			const char* const sourceCubicFragSh = 
				"void main(void)											"
				"{															"
				"	vec3 uv = gl_TexCoord[0].stp;							"
				"															"
				"	if( (uv.s*uv.s*uv.s - uv.t*uv.p)>=0.0 )					"
				"		discard;											"
				"															"
				"	gl_FragColor = vec4(1.0);								"
				"}															";

			const char* const sourceColorFillFragSh =
				"uniform vec4 uFillColor;									"
				"															"
				"void main()												"
				"{															"
				"	gl_FragColor = uFillColor;								"
				"}															";

			const char* const sourceFillVertSh =
				"uniform vec2 uImageDim;									"
				"															"
				"void main()												"
				"{															"
				"	gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;	"
				"	gl_TexCoord[0] = vec4(gl_Vertex.xy/uImageDim, 0, 0);	"
				"}															";

			const char* const sourcePatternFillFragSh =
				"uniform sampler2D uPattern;								"
				"															"
				"void main()												"
				"{															"
				"	gl_FragColor = tex2D(uPattern, gl_TexCoord[0].st);		"
				"}															";

			const char* const sourceStrokeVertSh =
				"attribute vec2 aOffset;									"
				"															"
				"void main()												"
				"{															"
				"	vec4 pos = gl_Vertex;									"
				"	pos.xy += 5*aOffset;										"
				"	gl_Position = gl_ModelViewProjectionMatrix*pos;			"
				"	gl_TexCoord[0] = gl_MultiTexCoord0;						"
				"}															";

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
				glDeleteShader(fsArc);

				prgMaskCubic = glCreateProgram();
				fsCubic = glCreateShader(GL_FRAGMENT_SHADER);
				CompileShader(fsCubic, sourceCubicFragSh);
				glAttachShader(prgMaskCubic, fsCubic);
				glLinkProgram(prgMaskCubic);
				glDeleteShader(fsCubic);

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
				
				glDeleteShader(fsQuad);
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
			}
		}
	}
}