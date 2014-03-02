#pragma once

#include "stdafx.h"
#include "camera.h"
#include "engine.inl"
#include <map>

#define CUBEXROWS 1
#define CUBEZROWS 1
#define CUBEYROWS 2

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

	GLuint mVertexBuffer;
	GLuint mCubeTextureName;
	vector<vertex_v3fv2f> mVertexData;
	unsigned int mVertexCount;

	void generateCube();
	void initializeCamera();
};

