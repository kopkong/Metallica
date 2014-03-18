#ifndef CAMERA_H
#define	CAMERA_H

#include "stdafx.h"
#include <math.h>

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))


class Camera
{
public:

    Camera(int WindowWidth, int WindowHeight,SDL_Window *window);

	void resetCamera(const glm::vec3& pos,const glm::vec3& direction, const glm::vec3& Up);

	glm::mat4 getMVP();
	glm::mat4 getView();
	glm::mat4 getProjection();
	void setModel(glm::mat4);

    void onKeyboard(SDL_Keycode Key);
	void onMouseDown(SDL_MouseButtonEvent button);
	void onMouseUp(SDL_MouseButtonEvent button);
	void onMouseMotion(SDL_MouseMotionEvent motion);
	void onMouseWheel(SDL_MouseWheelEvent wheel);

    const glm::vec3& getPos() const
    {
        return mPos;
    }

	const glm::vec3& getLookatDirection() const
	{
		return mLookDirection;
	}

	void setCameraPosition(const glm::vec3& pos)
	{
		mPos = pos;
	}

private:
    void init();
	void initProjection();
	void initView();
	void zoomIn(); // move close
	void zoomOut(); // move far
	void rotateModelbyX();
	void rotateModelbyY();
	void rotateModelbyZ();
	void forward();
	void left();
	void right();
	void backward();
	void resetCameraDirection();
	void headup();
	void headdown();

    glm::vec3 mPos;
    glm::vec3 mLookDirection;
    glm::vec3 mUp;

	glm::mat4 mProjection;
	glm::mat4 mView;
	glm::mat4 mModel;

	SDL_Window *mSDLMainWindow;
    int mWindowWidth;
    int mWindowHeight;

	// Initial horizontal angle : toward -Z
	float mHorizontalAngle;
	// Initial vertical angle : none
	float mVerticalAngle;

    glm::ivec2 mMousePos;
};

#endif	/* CAMERA_H */