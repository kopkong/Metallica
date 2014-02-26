#pragma once

#include "stdafx.h"
#include "camera.h"
#include "engine.inl"

#define CUBEROWSIZE 3

class MagicCube
{
public:
	MagicCube(Camera*);
	~MagicCube(void);

	Camera *mCamera;
	

	void init();
	void exit();
	void render();
	void keyBoardHandler(SDL_Event*);

private:
	GLuint mProgName;
	GLuint mUniformMVP;
	GLuint mUniformCubeTexture;

	vertex_v3fv2f mVertexData;
	unsigned int mVertexCount;

	void generateCube();
};

