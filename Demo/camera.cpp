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

	keepRotationX = false;
	keepRotationY = false;
	keepRotationZ = false;
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
	switch (Key) {
	    case SDLK_x:
			keepRotationX = !keepRotationX;
			break;
		case SDLK_y:
			keepRotationY = !keepRotationY;
			break;
		case SDLK_z:
			keepRotationZ = !keepRotationZ;
			break;
		default:
			break;
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
	//SDL_Log("Mouse index : %d",button.button);
	//SDL_Log("Mouse state: %d",button.state);
	//SDL_Log("Motion state: %d",motion.state);
	if(motion.state == SDL_BUTTON_LMASK)
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
			
			rotateModel(hAngle,vAngle);
		}
	}
	else
	{
		mMousePos.x = 0;
		mMousePos.y = 0;
	}
}

void Camera::onMouseWheel(SDL_MouseWheelEvent wheel)
{
	if(wheel.y > 0)
		zoomIn();
	else
		zoomOut();
}

void Camera::onRender()
{
  
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

void Camera::rotateModel(float hAngle,float vAngle)
{
	mModel = rotate(mModel,hAngle,YAXIS);
	mModel = rotate(mModel,vAngle,XAXIS);
}

void Camera::rotateModelbyX()
{
	mModel = rotate(mModel,1.0f,XAXIS);
}

void Camera::rotateModelbyY()
{
	mModel = rotate(mModel,1.0f,YAXIS);
}

void Camera::rotateModelbyZ()
{
	mModel = rotate(mModel,1.0f,ZAXIS);
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

	if(keepRotationX)
		rotateModelbyX();
	if(keepRotationY)
		rotateModelbyY();
	if(keepRotationZ)
		rotateModelbyZ();

	// Our ModelViewProjection : multiplication of our 3 matrices
	glm::mat4 MVP        = Projection * View * mModel; // Remember, matrix multiplication is the other way around


	return MVP;
}