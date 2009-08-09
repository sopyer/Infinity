#include <glm\glmext.h>

class FirstPersonCamera
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
			//matrix *= /*matrix */ glm::translateGTX(-m_Position.x, -m_Position.y, -m_Position.z);
			matrix = /*matrix */ glm::translateGTX(m_Position.x, m_Position.y, -m_Position.z)*matrix;
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
		FirstPersonCamera()
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