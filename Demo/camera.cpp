#include "camera.h"
#include <gtc/matrix_transform.hpp>
using namespace glm;

const static float STEP_SCALE = 0.1f;
const static int MARGIN = 10;

Camera::Camera(int WindowWidth, int WindowHeight)
{
    mWindowWidth  = WindowWidth;
    mWindowHeight = WindowHeight;
    mPos          = glm::vec3(1.0f, -2.0f, -5.0f);
    mTarget       = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::normalize(mTarget);
    mUp           = glm::vec3(0.0f, 1.0f, 0.0f);

    //Init();
}


Camera::Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
    mWindowWidth  = WindowWidth;
    mWindowHeight = WindowHeight;
	resetCamera(Pos,Target,Up);
}

void Camera::init()
{
    glm::vec3 HTarget(mTarget.x, 0.0, mTarget.z);
    glm::normalize(HTarget);
    
    /*if (HTarget.z >= 0.0f)
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = 360.0f - ToDegree(asin(HTarget.z));
        }
        else
        {
            m_AngleH = 180.0f + ToDegree(asin(HTarget.z));
        }
    }
    else
    {
        if (HTarget.x >= 0.0f)
        {
            m_AngleH = ToDegree(asin(-HTarget.z));
        }
        else
        {
            m_AngleH = 90.0f + ToDegree(asin(-HTarget.z));
        }
    }
    
    m_AngleV = -ToDegree(asin(mTarget.y));*/

    m_OnUpperEdge = false;
    m_OnLowerEdge = false;
    m_OnLeftEdge  = false;
    m_OnRightEdge = false;
    //m_mousePos.x  = mWindowWidth / 2;
    //m_mousePos.y  = mWindowHeight / 2;

    //glutWarpPointer(m_mousePos.x, m_mousePos.y);
}

void Camera::resetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
	mPos = Pos;
    mTarget = Target;
	glm::normalize(mTarget);

    mUp = Up;
    glm::normalize(mUp);

    init();
}

bool Camera::onKeyboard(SDL_Keycode Key)
{
    bool Ret = false;

    switch (Key) {

	case SDLK_UP:
        {
            mPos += (mTarget * STEP_SCALE);
            Ret = true;
        }
        break;

	case SDLK_DOWN:
        {
            mPos -= (mTarget * STEP_SCALE);
            Ret = true;
        }
        break;

	case SDLK_LEFT:
        {
			glm::vec3 Left = glm::cross(mTarget,mUp);
			glm::normalize(Left);
            Left *= STEP_SCALE;
            mPos += Left;
            Ret = true;
        }
        break;

    case SDLK_RIGHT:
        {
            glm::vec3 Right = glm::cross(mUp,mTarget);
			glm::normalize(Right);
            Right *= STEP_SCALE;
            mPos += Right;
            Ret = true;
        }
        break;
    }

    return Ret;
}

void Camera::onMouseDown(SDL_MouseButtonEvent button)
{
	mMousePos.x = button.x;
	mMousePos.y = button.y;
}

void Camera::onMouseUp(SDL_MouseButtonEvent button)
{
	mMousePos = glm::ivec2(0,0);
}

void Camera::onMouseMotion(SDL_MouseMotionEvent motion)
{
	//if(motion.state == SDL_PRESSED)
	if(mMousePos.x + mMousePos.y > 0)
	{
		//SDL_Log("motion.x = %d",motion.x);
		//SDL_Log("motion.y = %d",motion.y);

		int x = motion.x;
		int y = motion.y;

		const int DeltaX = x - mMousePos.x;
		const int DeltaY = y - mMousePos.y;

		m_AngleH += (float)DeltaX / 20.0f;
		m_AngleV += (float)DeltaY / 20.0f;

		if (DeltaX == 0) {
			if (x <= MARGIN) {
			//    m_AngleH -= 1.0f;
				m_OnLeftEdge = true;
			}
			else if (x >= (mWindowWidth - MARGIN)) {
			//    m_AngleH += 1.0f;
				m_OnRightEdge = true;
			}
		}
		else {
			m_OnLeftEdge = false;
			m_OnRightEdge = false;
		}

		if (DeltaY == 0) {
			if (y <= MARGIN) {
				m_OnUpperEdge = true;
			}
			else if (y >= (mWindowHeight - MARGIN)) {
				m_OnLowerEdge = true;
			}
		}
		else {
			m_OnUpperEdge = false;
			m_OnLowerEdge = false;
		}

		mMousePos.x = x;
		mMousePos.y = y;

		update();
	}
}

void Camera::onRender()
{
    bool ShouldUpdate = false;

    if (m_OnLeftEdge) {
        m_AngleH -= 0.1f;
        ShouldUpdate = true;
    }
    else if (m_OnRightEdge) {
        m_AngleH += 0.1f;
        ShouldUpdate = true;
    }

    if (m_OnUpperEdge) {
        if (m_AngleV > -90.0f) {
            m_AngleV -= 0.1f;
            ShouldUpdate = true;
        }
    }
    else if (m_OnLowerEdge) {
        if (m_AngleV < 90.0f) {
           m_AngleV += 0.1f;
           ShouldUpdate = true;
        }
    }

    if (ShouldUpdate) {
        update();
    }
}

void Camera::update()
{
    const glm::vec3 Vaxis(0.0f, 1.0f, 0.0f);

    // Rotate the view vector by the horizontal angle around the vertical axis
    glm::vec3 View(1.0f, 0.0f, 0.0f);	
    //View.Rotate(m_AngleH, Vaxis);
    //View.Normalize();

    // Rotate the view vector by the vertical angle around the horizontal axis
	glm::vec3 Haxis = glm::cross(Vaxis,View);
	glm::normalize(Haxis);
    //View.Rotate(m_AngleV, Haxis);
       
    mTarget = View;
	glm::normalize(mTarget);

	mUp = glm::cross(mTarget,Haxis);
    glm::normalize(mUp);
}

glm::mat4 Camera::getMVP()
{
	// Projection matrix : 45?Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								mPos,
								mTarget,
								mUp  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);
	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around


	return MVP;
}