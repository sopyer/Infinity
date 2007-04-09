#include "Engine.h"
#include "Pipeline.h"
#include <cstring>
#include "Buffer.h"
#include "PrimitiveRenderer.h"
#include "Texture.h"
#include <gl\glaux.h>
#include "Loader.h"
#include "glm\glm.h"
#include "glm\glmext.h"
#include "debug.h"
#include "ArcBall.h"

#pragma comment (lib,"OpenGL32.Lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"WinMm.Lib")

class MyEngine: public Engine
{
public:
	MyEngine();
	void OnCreate();
	void OnRender();
	void OnDestroy();

	bool bLook;

	void OnMouseDown(float x, float y, uint32 state)
	{
		if( state & MK_LBUTTON)
		{
			bLook = true;
			mCore.getCursor()->set(coreParams.mWidth / 2, coreParams.mHeight / 2);
			mCore.getCursor()->show(false);
		}
		if( state & MK_RBUTTON)
		{
			bLook = false;
			mCore.getCursor()->show(true);
		}
	}
	void OnMouseMove(float x, float y)
	{
		if(bLook)
		{
			glm::vec2 pos = mCore.getCursor()->get();
			glm::vec2 ang = (pos - glm::vec2(coreParams.mWidth / 2, coreParams.mHeight / 2))*0.4f;
			glm::clamp(ang, glm::vec2(-45, -45), glm::vec2(45, 45));
			m_Camera.rotate(ang.x, ang.y);
			m_Camera.set();
			mCore.getCursor()->set(coreParams.mWidth / 2, coreParams.mHeight / 2);
		}
	}
public:
	ShaderObject *vs, *fs;

	GPUProgram po;
	Texture *tex0, *tex1;

	PrimitiveRenderer rend;
	
	ArcBall	m_Camera;
	Loader loader;
};

int WINAPI WinMain(	HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	MyEngine	engine;
	return engine.MainLoop();
}

MyEngine::MyEngine():bLook(0){}

void MyEngine::OnRender (void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	rend.SetProgram(&po);
	rend.AddVertexAttrStream(loader.vert_buf, loader.num_decl, loader.Decl);
	rend.SetIndexStream(loader.ind_buf, GL_UNSIGNED_SHORT);
	rend.SetTexture(0, tex0);
	rend.SetTexture(1, tex1);

	rend.DrawElements(GL_TRIANGLES, loader.num_ind);
	
	rend.SetProgram(0);

	const float ww=1;
	glBegin(GL_LINES);
		glColor3f(1,0,0);
		glVertex3f(0,0,0);
		glVertex3f(ww,0,0);

		glColor3f(0,1,0);
		glVertex3f(0,0,0);
		glVertex3f(0,ww,0);

		glColor3f(0,0,1);
		glVertex3f(0,0,0);
		glVertex3f(0,0,ww);
	glEnd();
	for(float i = -500; i <= 500; i += 5)
	{
		glBegin(GL_LINES);
			glColor3ub(150, 190, 150);						
			glVertex3f(-500, 0, i);					
			glVertex3f(500, 0, i);
			glVertex3f(i, 0,-500);							
			glVertex3f(i, 0, 500);
		glEnd();
	}
	glColor3f(1,1,1);
	glFlush();
}

void MyEngine::OnCreate()
{
	m_RM.AddResource("Texture", "jpeg", "Fieldstone.jpg");
	m_RM.AddResource("Bumpmap", "jpeg", "FieldstoneBumpDOT3.jpg");
	m_RM.AddResource("VertProg", "vert", "SHL_PP_D3BM.vert");
	m_RM.AddResource("FragProg", "frag", "SHL_PP_D3BM.frag");
	m_RM.LoadAll();
	
	tex0 = 0;
	tex0 = dynamic_cast<Image*>(m_RM.GetResource("Texture").get())->GetTexture();
	tex1 = dynamic_cast<Image*>(m_RM.GetResource("Bumpmap").get())->GetTexture();
	vs = dynamic_cast<Shader*>(m_RM.GetResource("VertProg").get())->GetShader();
	fs = dynamic_cast<Shader*>(m_RM.GetResource("FragProg").get())->GetShader();
	 
	po.AddShader(vs);	 
	po.AddShader(fs);	 
	po.Link();
	m_Log.Print(Log::Info, po.GetLinkerLog());
	
	GLuint tangent =   po.BindVertexAttrib("rm_Tangent");
	GLuint bitangent = po.BindVertexAttrib("rm_Binormal");
	loader.Load(m_VFS.Open("teapot.chunk"), tangent, bitangent);

	GLuint sp = po.GetProgram();
	//glValidateProgram(sp); 
	//m_Log.Print(Log::Info, po.GetLinkerLog());
	glUseProgram(sp);

	GLuint sampler0 = po.BindVar("rgb_map");
	GLuint sampler1 = po.BindVar("bump_map");
	GLuint rmat = po.BindVar("r_sh_matrix");
	GLuint gmat = po.BindVar("g_sh_matrix");
	GLuint bmat = po.BindVar("b_sh_matrix");

	po.SetVar1i(sampler0, 0);
	po.SetVar1i(sampler1, 1);

	po.SetVarMatrix4fv(rmat, 1, GL_FALSE, 
		glm::mat4(
		0.09010f, -0.04719f, 0.24026f, -0.14838f,
		-0.04719f, -0.09010f, -0.11155f, 0.19955f,
		0.24026f, -0.11155f, -0.11890f, -0.17397f,
		-0.14838f, 0.19955f, -0.17397f, -0.07239f
		));

	po.SetVarMatrix4fv(gmat, 1, GL_FALSE, 
		glm::mat4(
		-0.02145f, -0.02145f, 0.09010f, -0.03070f,
		-0.02145f, 0.02145f, -0.09439f, 0.17908f,
		0.09010f, -0.09439f, -0.06688f, -0.09210f,
		-0.03070f, 0.17908f, -0.09210f, -0.01280f
		));

	po.SetVarMatrix4fv(bmat, 1, GL_FALSE, 
		glm::mat4(
		-0.12871f, -0.05149f, 0.06007f, 0.00512f,
		-0.05149f, 0.12871f, -0.20165f, 0.30700f,
		0.06007f, -0.20165f, -0.11147f, -0.13815f,
		0.00512f, 0.30700f, -0.13815f, -0.03463f
		));

	m_Camera.m_MaxForwardVelocity = 5.0f;
	m_Camera.m_MaxPitchRate = 5.0f;
	m_Camera.m_MaxHeadingRate = 5.0f;
	m_Camera.m_PitchDegrees = -10.0f;
	m_Camera.m_HeadingDegrees = 45.0f;
	m_Camera.m_Position = glm::vec3(-3, 1, -3);
	m_Camera.set();
}

void MyEngine::OnDestroy()
{
	loader.Unload();
	m_RM.UnloadAll();
}