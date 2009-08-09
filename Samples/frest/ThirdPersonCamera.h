//! Quick hack
#pragma once

#include <glm\glmext.h>

class ThirdPersonCamera
{
	public:
		ThirdPersonCamera(): mPhiAngle(0), mPsiAngle(0)
		{}

		void rotate(float phi, float psi)
		{
			mPhiAngle += phi;
			mPsiAngle += psi;

			if (mPhiAngle >=  360.0f) mPhiAngle -= 360.0f;
			if (mPhiAngle <= -360.0f) mPhiAngle += 360.0f;
			if (mPsiAngle >=  360.0f) mPsiAngle -=360.0f;
			if (mPsiAngle <= -360.0f) mPsiAngle +=360.0f;
		}

		glm::mat4 getMatrix()
		{
			glm::mat4 matView(1.0f);
			
			matView *= glm::translateGTX(0.0f, 0.0f, -6.0f); //Make distance value a parameter
			matView *= glm::rotateGTX(mPhiAngle, 1.0f, 0.0f, 0.0f);
			matView *= glm::rotateGTX(mPsiAngle, 0.0f, 1.0f, 0.0f);

			return matView;
		}

	private:
		float		mPhiAngle, mPsiAngle;
};