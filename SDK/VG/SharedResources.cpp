#include <gl\glee.h>
#include <cassert>
#include "SharedResources.h"

namespace vg
{
	namespace shared
	{
		GLuint	prgMaskQuad = 0;
		GLuint	prgMaskCubic = 0;
		GLuint	prgMaskArc = 0;	
		GLuint	prgFillColor = 0;
		GLuint	prgFillPattern = 0;

		namespace
		{
			size_t refCount = 0;

			const char* const sourceArcFragSh = 
				"void main(void)											"
				"{															"
				"	vec2 uv = gl_TexCoord[0].st;							"
				"															"
				"	if( (uv.s*uv.s + uv.t*uv.t)>1.0 )						"
				"		discard;											"
				"															"
				"	gl_FragColor = vec4(1.0);								"
				"}															";

			const char* const sourceQuadFragSh = 
				"void main(void)											"
				"{															"
				"	vec2 uv = gl_TexCoord[0].st;							"
				"															"
				"	if( (pow(uv.s, 2.0)-uv.t)>0.0 )							"
				"		discard;											"
				"															"
				"	gl_FragColor = vec4(1);									"
				"}															";

			const char* const sourceCubicFragSh = 
				"void main(void)											"
				"{															"
				"	vec3 uv = gl_TexCoord[0].stp;							"
				"															"
				"	if( (pow(uv.s, 3.0)-uv.t*uv.p)>0.0 )					"
				"		discard;											"
				"															"
				"	gl_FragColor = vec4(1);									"
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
				glDeleteShader(fsQuad);
				
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
			}
		}
	}
}