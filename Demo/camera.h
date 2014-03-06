#ifndef CAMERA_H
#define	CAMERA_H

#include "stdafx.h"
#include <math.h>

#define ToRadian(x) (float)(((x) * M_PI / 180.0f))
#define ToDegree(x) (float)(((x) * 180.0f / M_PI))


class Camera
{
public:

    Camera(int WindowWidth, int WindowHeight);

    Camera(int WindowWidth, int WindowHeight, const glm::vec3& Pos, const glm::vec3& Target, const glm::vec3& Up);

	void resetCamera(const glm::vec3& pos,const glm::vec3& Target, const glm::vec3& Up);

	glm::mat4 getMVP();
	glm::mat4 getView();
	glm::mat4 getProjection();
	void setModel(glm::mat4);

    void onKeyboard(SDL_Keycode Key);
	void onMouseDown(SDL_MouseButtonEvent button);
	void onMouseUp(SDL_MouseButtonEvent button);
	void onMouseMotion(SDL_MouseButtonEvent event, SDL_MouseMotionEvent motion);
	void onMouseWheel(SDL_MouseWheelEvent wheel);

    const glm::vec3& getPos() const
    {
        return mPos;
    }

    const glm::vec3& getTarget() const
    {
        return mTarget;
    }

    const glm::vec3& getUp() const
    {
        return mUp;
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

    glm::vec3 mPos;
    glm::vec3 mTarget;
    glm::vec3 mUp;

	glm::mat4 mProjection;
	glm::mat4 mView;
	glm::mat4 mModel;

    int mWindowWidth;
    int mWindowHeight;

	// Initial horizontal angle : toward -Z
	float mHorizontalAngle;
	// Initial vertical angle : none
	float mVerticalAngle;

    glm::ivec2 mMousePos;
};

#endif	/* CAMERA_H */