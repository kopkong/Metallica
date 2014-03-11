#pragma once

#include "stdafx.h"
#include "camera.h"
#include "engine.inl"
#include <map>

#define CUBEXROWS 4
#define CUBEZROWS 4
#define CUBEYROWS 4

struct MotionCube
{
	MotionCube(glm::vec3 pos, glm::vec3 dir, float v, float t)
	{
		StartPos = pos;
		MoveDirection = dir;
		Speed = v;
		ElapsedTime = t;
	};

	glm::vec3 StartPos;
	glm::vec3 MoveDirection;
	float Speed;
	float ElapsedTime;
};

class MagicCube
{
public:
	MagicCube(Camera*);
	~MagicCube(void);

	Camera *mCamera;
	
	void update();
	void init();
	void exit();
	void render();
	void onKeyboard(SDL_Keycode Key);
	void onMouseDown(SDL_MouseButtonEvent);

private:
	GLuint mProgCubeRendering;
	GLuint mProgSkyBox;
	GLuint mProgRayTest;
	GLuint mUniformMVP;
	GLuint mUniformCubeTexture;
	GLuint mUniformSkyBoxTexture;
	GLuint mUniformRayTestMVP;
	GLuint mUniformCameraPosition;
	GLuint mUniformViewProjectMatrix;

	GLuint mVertexBuffer;
	GLuint mVertexSkyBox;
	GLuint mTextureCubeFace;
	GLuint mTextureInstanceOffsetBuffer;
	GLuint mCubeMapSkyBox;

	vector<vertex_v3fv2f> mVertexData;
	vector<MotionCube> mMotionCubes;
	vector<glm::mat4> mModelCoordinateData;
	glm::mat4 mSkyBoxCoordinateData;
	unsigned int mInstanceCount;
	unsigned int mVertexCount;
	int mSelectedInstance;
	bool mIsPicking;
	glm::vec3 mRayOrigin;
	glm::vec3 mRayDirection;

	bool mKeepRotateX;
	bool mKeepRotateY;
	bool mKeepRotateZ;
	glm::ivec2 mMousePos;
	glm::ivec2 mScreenSize;

	void rotateModel(glm::mat4 &model);
	void generateCube();
	void initializeCamera();
	void screenPosToWorldRay(int mouseX,int mouseY,int screenWidth,int screenHeight,
		glm::mat4 viewMatrix,glm::mat4 projMatrix,glm::vec3& out_origin,glm::vec3& out_direction);
	bool testRayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction,glm::vec3 aabb_min,
		glm::vec3 aabb_max, glm::mat4 modelMatrix,float& intersectionDistance);

	void renderRollingCube();
	void renderRayTest();
	void renderCube();
	void renderSkyBox();
};

