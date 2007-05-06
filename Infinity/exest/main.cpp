#include <framework.h>
#include <glm\glmext.h>

class FirstPersonCamera1
{
	public:
		GLfloat m_MaxPitchRate;
		GLfloat m_MaxHeadingRate;
		GLfloat m_HeadingDegrees;
		GLfloat m_PitchDegrees;
		GLfloat m_MaxForwardVelocity;
		GLfloat m_ForwardVelocity;
		glm::__quatGTX m_qHeading;
		glm::__quatGTX m_qPitch;
		glm::vec3 m_Position;
		glm::vec3 m_DirectionVector;


		glm::vec3 look, up, right, position; 
		glm::mat4 matrix;

		void SetPerspective()
		{
			glm::mat4 Matrix;
			glm::__quatGTX q;

			// Make the Quaternions that will represent our rotations
			m_qPitch = glm::angleAxisGTX(m_PitchDegrees, 1.0f, 0.0f, 0.0f );
			m_qHeading = glm::angleAxisGTX(m_HeadingDegrees, 0.0f, 1.0f, 0.0f);
			
			// Combine the pitch and heading rotations and store the results in q
			q = glm::crossGTX(m_qPitch, m_qHeading);
			Matrix = matrix = glm::mat4GTX(q);

			// Let OpenGL set our new prespective on the world!
			//glMultMatrixf(Matrix);
			
			// Create a matrix from the pitch Quaternion and get the j vector 
			// for our direction.
			Matrix = glm::mat4GTX(m_qPitch);
			m_DirectionVector.y = Matrix[1][2];

			// Combine the heading and pitch rotations and make a matrix to get
			// the i and j vectors for our direction.
			q = m_qHeading * m_qPitch;
			Matrix = glm::mat4GTX(q);
			m_DirectionVector.x = Matrix[0][2];
			m_DirectionVector.z = Matrix[2][2];

			// Scale the direction by our speed.
			m_DirectionVector *= m_ForwardVelocity;

			// Increment our position by the vector
			m_Position.x += m_DirectionVector.x;
			m_Position.y += m_DirectionVector.y;
			m_Position.z += m_DirectionVector.z;

			// Translate to our new position.
			//glTranslatef(-m_Position.x, -m_Position.y, m_Position.z);
			matrix *= /*matrix */ glm::translateGTX(-m_Position.x, -m_Position.y, -m_Position.z);
			//matrix = /*matrix */ glm::translateGTX(m_Position.x, m_Position.y, -m_Position.z)*matrix;
		}

		void ChangePitch(GLfloat degrees)
		{
			if(fabs(degrees) < fabs(m_MaxPitchRate))
			{
				// Our pitch is less than the max pitch rate that we 
				// defined so lets increment it.
				m_PitchDegrees += degrees;
			}
			else
			{
				// Our pitch is greater than the max pitch rate that
				// we defined so we can only increment our pitch by the 
				// maximum allowed value.
				if(degrees < 0)
				{
					// We are pitching down so decrement
					m_PitchDegrees -= m_MaxPitchRate;
				}
				else
				{
					// We are pitching up so increment
					m_PitchDegrees += m_MaxPitchRate;
				}
			}

			// We don't want our pitch to run away from us. Although it
			// really doesn't matter I prefer to have my pitch degrees
			// within the range of -360.0f to 360.0f
			if(m_PitchDegrees > 360.0f)
			{
				m_PitchDegrees -= 360.0f;
			}
			else if(m_PitchDegrees < -360.0f)
			{
				m_PitchDegrees += 360.0f;
			}
		}

		void ChangeHeading(GLfloat degrees)
		{
			if(fabs(degrees) < fabs(m_MaxHeadingRate))
			{
				// Our Heading is less than the max heading rate that we 
				// defined so lets increment it but first we must check
				// to see if we are inverted so that our heading will not
				// become inverted.
				if(m_PitchDegrees > 90 && m_PitchDegrees < 270 || (m_PitchDegrees < -90 && m_PitchDegrees > -270))
				{
					m_HeadingDegrees -= degrees;
				}
				else
				{
					m_HeadingDegrees += degrees;
				}
			}
			else
			{
				// Our heading is greater than the max heading rate that
				// we defined so we can only increment our heading by the 
				// maximum allowed value.
				if(degrees < 0)
				{
					// Check to see if we are upside down.
					if((m_PitchDegrees > 90 && m_PitchDegrees < 270) || (m_PitchDegrees < -90 && m_PitchDegrees > -270))
					{
						// Ok we would normally decrement here but since we are upside
						// down then we need to increment our heading
						m_HeadingDegrees += m_MaxHeadingRate;
					}
					else
					{
						// We are not upside down so decrement as usual
						m_HeadingDegrees -= m_MaxHeadingRate;
					}
				}
				else
				{
					// Check to see if we are upside down.
					if(m_PitchDegrees > 90 && m_PitchDegrees < 270 || (m_PitchDegrees < -90 && m_PitchDegrees > -270))
					{
						// Ok we would normally increment here but since we are upside
						// down then we need to decrement our heading.
						m_HeadingDegrees -= m_MaxHeadingRate;
					}
					else
					{
						// We are not upside down so increment as usual.
						m_HeadingDegrees += m_MaxHeadingRate;
					}
				}
			}
			
			// We don't want our heading to run away from us either. Although it
			// really doesn't matter I prefer to have my heading degrees
			// within the range of -360.0f to 360.0f
			if(m_HeadingDegrees > 360.0f)
			{
				m_HeadingDegrees -= 360.0f;
			}
			else if(m_HeadingDegrees < -360.0f)
			{
				m_HeadingDegrees += 360.0f;
			}
		}

		void ChangeVelocity(GLfloat vel)
		{
			if(fabs(vel) < fabs(m_MaxForwardVelocity))
			{
				// Our velocity is less than the max velocity increment that we 
				// defined so lets increment it.
				m_ForwardVelocity += vel;
			}
			else
			{
				// Our velocity is greater than the max velocity increment that
				// we defined so we can only increment our velocity by the 
				// maximum allowed value.
				if(vel < 0)
				{
					// We are slowing down so decrement
					m_ForwardVelocity -= -m_MaxForwardVelocity;
				}
				else
				{
					// We are speeding up so increment
					m_ForwardVelocity += m_MaxForwardVelocity;
				}
			}
		}
	public:
		FirstPersonCamera1()
		{
			// Initalize all our member varibles.
			m_MaxPitchRate			= 180.0f;
			m_MaxHeadingRate		= 360.0f;
			m_HeadingDegrees		= 180.0f;
			m_PitchDegrees			= 0.0f;
			m_MaxForwardVelocity	= 0.0f;
			m_ForwardVelocity		= 0.0f;
		}
		void move(glm::vec3 dir)
		{
			m_Position += dir;
		}
		void rotate(float vert, float hor)
		{
			ChangePitch(hor);
			ChangeHeading(vert);
		}
		glm::mat4 getMatrix()
		{
			SetPerspective();
			return matrix;
		}
};

#include <DataPtr.h>

#define MESH_INDICES_MAGIC '10im'

VertexDecl SubMesh::decl_[3]=
{
	{USAGE_POSITION, 3, GL_FLOAT, 0, 8*4},
	{USAGE_NORMAL, 3, GL_FLOAT, 3*4, 8*4},
	{USAGE_TEXCOORD0, 2, GL_FLOAT, 6*4, 8*4}
};



MeshPtr loadMesh1(const char* path)
{
	int magic;
	MeshPtr mesh;
	DataStreamPtr stream = vfsMapFile(path);
	if(!stream)
		return mesh;
	
	DataPtr ptr(stream->MapStream(), stream->GetSize());

	ptr.read(magic);
	
	if(magic == MESH_INDICES_MAGIC) 
	{
		int num_surfaces;
		ptr.read(num_surfaces);
		mesh = MeshPtr(new Mesh);
		for(int i = 0; i < num_surfaces; i++) {
			SubMesh *s = new SubMesh();
			
			ptr.move(128);

			ptr.read(s->numVert_);
			s->vert_.setBufferData(8*4*s->numVert_, ptr.move(8*4*s->numVert_));

			ptr.read(s->numInd_);
			s->numInd_*=3;
			s->ind_.setBufferData(s->numInd_*4, ptr.move(s->numInd_*4));
			mesh->push_back(SubMeshPtr(s));
		}
	}
	return mesh;
}

VertexDecl decl = {USAGE_POSITION, 3, GL_FLOAT, 0, 0};

class Exest: public Framework
{
	private:
		GLint timeLoc, tex1Loc, tex0Loc;
		glRenderer	renderer_;
		glTexture2DPtr	image_[2];
		glProgramPtr	prog_;
		MeshPtr		mesh;
		FirstPersonCamera1	camera;
	protected:
		void OnCreate()
		{
			vfsAddRoot("D:\\Temp\\ExestData");
			mesh = loadMesh1("sky.mesh");
			image_[0] = glTexture2DPtr(loadPngTexture("horizon.png"));
			image_[0]->setWrapMode(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
			image_[1] = glTexture2DPtr(loadPngTexture("clouds.png"));
			prog_ = glProgramPtr(loadProgram("sky.vert;sky.frag"));
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(90.0, (float)width_/height_,1,2048);
			glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
			glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
			glClearDepth(1.0f);									// Depth Buffer Setup
			glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
			glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
			timeLoc = prog_->getUniformLocation("time");
			tex0Loc = prog_->getUniformLocation("tex0");
			tex1Loc = prog_->getUniformLocation("tex1");
			renderer_.useProgram(prog_.get());
			renderer_.setUniform1f(timeLoc, glfwGetTime());
			renderer_.setUniform1i(tex0Loc, 0);
			renderer_.setUniform1i(tex1Loc, 1);
			renderer_.useProgram(0);
			//camera.rotate(180, 0);
			//camera.move(glm::vec3(0, 1, -4)); 
		}
		
		void OnRender()
		{
			glClearDepth(1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			glTranslatef(0.0f, -1.0f, -4.0f);
			renderer_.beginRenderPass();
				glEnable(GL_DEPTH_TEST);
				glColor3f(1.0f, 1.0f, 1.0f);
				renderer_.setTexture(GL_TEXTURE0, image_[0].get());
				renderer_.setTexture(GL_TEXTURE1, image_[1].get());
				renderer_.useProgram(prog_.get());
				renderer_.setUniform1f(timeLoc, glfwGetTime());
				glPushMatrix();
					glMultMatrixf(glm::mat4(1,0,0,0,
											0,0,1,0,
											0,1,0,0,
											0,0,0,1));
					glTranslatef(0,0,-400);
					renderer_.addAttribBuffer((*mesh)[0]->vert_, 1, SubMesh::decl_);
					renderer_.setIndexBuffer((*mesh)[0]->ind_, GL_UNSIGNED_INT);
					renderer_.drawPrimitives(GL_TRIANGLES, (*mesh)[0]->numInd_);
				glPopMatrix();
				renderer_.useProgram(0);
				glDisable(GL_TEXTURE_2D);
				for(float i = -10; i <= 10; i += 1)
				{
					glBegin(GL_LINES);
						glColor3ub(68, 193, 181);						
						glVertex3f(-10, 0, i);					
						glVertex3f(10, 0, i);
						glVertex3f(i, 0,-10);							
						glVertex3f(i, 0, 10);
					glEnd();
				}
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
			renderer_.endRenderPass();
			glFlush();
		}

		void OnUpdate(float frame_time)
		{
			if( glfwGetKey(GLFW_KEY_ESC) )
				close();
			if( glfwGetKey(GLFW_KEY_KP_8) )
				camera.rotate(0, -0.5);
			if( glfwGetKey(GLFW_KEY_KP_2) )
				camera.rotate(0, 0.5);
			if( glfwGetKey(GLFW_KEY_KP_4) )
				camera.rotate(-0.5, 0);
			if( glfwGetKey(GLFW_KEY_KP_6) )
				camera.rotate(0.5, 0);
			if( glfwGetKey('W') )
				camera.move(glm::vec3(0.0,0.0,-0.1));
			if( glfwGetKey('S') )
				camera.move(glm::vec3(0.0,0.0,0.1));
			if( glfwGetKey('A') )
				camera.move(glm::vec3(0.1,0.0,0.0));
			if( glfwGetKey('D') )
				camera.move(glm::vec3(-0.1,0.0,0.0));
		}
};

int main()
{
	Exest app;
	app.run();
	return 0;
}