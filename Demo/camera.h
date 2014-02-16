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

    bool onKeyboard(SDL_Keycode Key);

	void onMouseDown(SDL_MouseButtonEvent button);
	void onMouseUp(SDL_MouseButtonEvent button);

	void onMouseMotion(SDL_MouseMotionEvent motion);

    void onRender();

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
    void update();

    glm::vec3 mPos;
    glm::vec3 mTarget;
    glm::vec3 mUp;

    int mWindowWidth;
    int mWindowHeight;

    float m_AngleH;
    float m_AngleV;

    bool m_OnUpperEdge;
    bool m_OnLowerEdge;
    bool m_OnLeftEdge;
    bool m_OnRightEdge;

    glm::ivec2 mMousePos;
};

#endif	/* CAMERA_H */