#include "camera.h"
#include <gtc/matrix_transform.hpp>
using namespace glm;

const static float STEP_SCALE = 0.3f;
const static int MARGIN = 10;
const glm::vec3 YAXIS = glm::vec3(0.0,1.0,0.0);
const glm::vec3 XAXIS = glm::vec3(1.0,0.0,0.0);
const glm::vec3 ZAXIS = glm::vec3(0.0,0.0,1.0);

Camera::Camera(int WindowWidth, int WindowHeight)
{
    mWindowWidth  = WindowWidth;
    mWindowHeight = WindowHeight;
    mPos          = glm::vec3(1.0f, -2.0f, -5.0f);
    mTarget       = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::normalize(mTarget);
    mUp           = glm::vec3(0.0f, 1.0f, 0.0f);

    mModel   = glm::mat4(1.0f);
	// Initial horizontal angle : toward -Z
	mHorizontalAngle = 3.14f;
	// Initial vertical angle : none
	mVerticalAngle = 0.0f;
}

void Camera::init()
{
   
}

void Camera::resetCamera(const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up)
{
	mPos = Pos;
    mTarget = Target;
	glm::normalize(mTarget);

    mUp = Up;
    glm::normalize(mUp);

}

void Camera::onKeyboard(SDL_Keycode Key)
{
	glm::vec3 direction = normalize(mPos-mTarget);
	glm::vec3 right = cross(direction,mUp);

	switch(Key)
	{
	case SDLK_LEFT:
		mPos -= right;
		break;
	case SDLK_RIGHT:
		mPos += right;
		break;
	default:break;
	}

}

void Camera::onMouseDown(SDL_MouseButtonEvent button)
{

}

void Camera::onMouseUp(SDL_MouseButtonEvent button)
{
	//mMousePos.x = 0;
	//mMousePos.y = 0;
}

void Camera::onMouseMotion(SDL_MouseButtonEvent button,SDL_MouseMotionEvent motion)
{
	//SDL_Log("Mouse index : %d",button.button);
	//SDL_Log("Mouse state: %d",button.state);
	//SDL_Log("Motion state: %d",motion.state);
	/*if(motion.state == SDL_BUTTON_MMASK)
	{
		if(mMousePos.x == 0 && mMousePos.y == 0)
		{
			mMousePos.x = button.x;
			mMousePos.y = button.y;
		}
		else
		{
			int deltaX = button.x - mMousePos.x;
			int deltaY = button.y - mMousePos.y;

			float hAngle = deltaY > 0 ? -1.0f: 1.0f;
			float vAngle = deltaX > 0 ? 1.0f: -1.0f;

		}
	}
	else
	{
		mMousePos.x = 0;
		mMousePos.y = 0;
	}*/
}

void Camera::onMouseWheel(SDL_MouseWheelEvent wheel)
{
	if(wheel.y > 0)
		zoomIn();
	else
		zoomOut();
}

void Camera::zoomIn()
{
	glm::vec3 direction = normalize(mPos - mTarget);
	mPos -= direction * STEP_SCALE;
}

void Camera::zoomOut()
{
	glm::vec3 direction = normalize(mPos - mTarget);
	mPos += direction * STEP_SCALE;
}

void Camera::initProjection()
{
	mProjection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
}

void Camera::initView()
{
	mView = glm::lookAt(mPos,mTarget,mUp);
}

void Camera::setModel(glm::mat4 model)
{
	mModel = model;
}

glm::mat4 Camera::getView()
{
	initView();
	return mView;
}

glm::mat4 Camera::getProjection()
{
	initProjection();
	return mProjection;
}

glm::mat4 Camera::getMVP()
{
	initProjection();
	initView();

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = mProjection * mView * mModel; // Remember, matrix multiplication is the other way around

	return MVP;
}