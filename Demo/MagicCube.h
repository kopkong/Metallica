#pragma once

#include "stdafx.h"
#include "camera.h"
#include "engine.inl"
#include <map>

#define CUBEXROWS 2
#define CUBEZROWS 2
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
	GLuint mUniformTextureBufferOffset;

	GLuint mVertexBuffer;
	GLuint mInstancedVertexOffsetBuffer;
	GLuint mTextureCubeFace;
	GLuint mTextureInstanceOffsetBuffer;

	vector<vertex_v3fv2f> mVertexData;
	vector<glm::vec3> mInstancedOffsetData;
	unsigned int mInstanceCount;
	unsigned int mVertexCount;

	void generateCube();
	void initializeCamera();
};

