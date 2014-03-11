#include "camera.h"
#include <gtc/matrix_transform.hpp>
using namespace glm;

const static float STEP_SCALE = 0.3f;
const static int MARGIN = 10;
const glm::vec3 YAXIS = glm::vec3(0.0,1.0,0.0);
const glm::vec3 XAXIS = glm::vec3(1.0,0.0,0.0);
const glm::vec3 ZAXIS = glm::vec3(0.0,0.0,1.0);
const float MoveSpeed = 0.5;
const float MouseSpeed = 0.0008f;

Camera::Camera(int WindowWidth, int WindowHeight,SDL_Window * window)
{
    mWindowWidth		= WindowWidth;
    mWindowHeight		= WindowHeight;
	mSDLMainWindow     = window;

    mPos						 = glm::vec3(0.0f, 10.0f, -10.0f);
    mLookDirection			 = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::normalize(mLookDirection);
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

void Camera::resetCamera(const glm::vec3& Pos, const glm::vec3& direction, const glm::vec3& Up)
{
	mPos = Pos;
    mLookDirection = direction;
	glm::normalize(direction);

    mUp = Up;
    glm::normalize(mUp);

}

void Camera::onKeyboard(SDL_Keycode Key)
{
	// move the camera
	// w,s,a,d or
	// Up Down Left Right
	switch(Key)
	{
	case SDLK_UP:
		forward();
		break;
	case SDLK_LEFT:
		left();
		break;
	case SDLK_RIGHT:
		right();
		break;
	case SDLK_DOWN:
		backward();
		break;
	case SDLK_w:
		forward();
		break;
	case SDLK_s:
		backward();
		break;
	case SDLK_a:
		left();
		break;
	case SDLK_d:
		right();
		break;

	// Camera control
	case SDLK_HOME:
		resetCameraDirection();
		break;
	case SDLK_PAGEUP:
		headup();
		break;
	case SDLK_PAGEDOWN:
		headdown();
		break;

	default:break;
	}

}

void Camera::onMouseDown(SDL_MouseButtonEvent button)
{

}

void Camera::onMouseUp(SDL_MouseButtonEvent button)
{
	mMousePos.x = 0;
	mMousePos.y = 0;
}

void Camera::onMouseMotion(SDL_MouseButtonEvent button,SDL_MouseMotionEvent motion)
{
	//if(motion.state == SDL_BUTTON_RMASK)
	{
		if(mMousePos.x == 0 && mMousePos.y == 0)
		{
			mMousePos.x = button.x;
			mMousePos.y = button.y;
		}

		float horizontalDistance = MouseSpeed * float(mMousePos.x - button.x);
		float verticalDistance = MouseSpeed * float(mMousePos.y -button.y);

		mLookDirection += glm::vec3(horizontalDistance,verticalDistance,0);
	}

	mMousePos.x = button.x;
	mMousePos.y = button.y;
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
	//glm::vec3 direction = normalize(mPos - mTarget);
	//mPos -= direction * STEP_SCALE;
}

void Camera::zoomOut()
{
	//glm::vec3 direction = normalize(mPos - mTarget);
	//mPos += direction * STEP_SCALE;
}

void Camera::forward()
{
	mPos += MoveSpeed * mLookDirection;
}

void Camera::backward()
{
	mPos -= MoveSpeed* mLookDirection;
}

void Camera::left()
{
	glm::vec3 right = cross(mLookDirection,mUp);
	mPos -= MoveSpeed * right;
}

void Camera::right()
{
	glm::vec3 right = cross(mLookDirection,mUp);
	mPos += MoveSpeed * right;
}

void Camera::headup()
{
	mVerticalAngle += 0.1f;
}

void Camera::headdown()
{
	mVerticalAngle -= 0.1f;
}

void Camera::resetCameraDirection()
{
	// reset direction to Z+
	mLookDirection = glm::vec3(0.0,0.0,1.0);
}

void Camera::initProjection()
{
	mProjection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
}

void Camera::initView()
{
	glm::vec3 targetPos = mPos + mLookDirection;
	mView = glm::lookAt(mPos,targetPos,mUp);
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