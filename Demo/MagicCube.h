#pragma once

#include "stdafx.h"
#include "camera.h"
#include "engine.inl"
#include <map>

#define CUBEXROWS 3
#define CUBEZROWS 3
#define CUBEYROWS 3

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
	vector<glm::mat4> mModelCoordinateData;
	unsigned int mInstanceCount;
	unsigned int mVertexCount;

	bool mKeepRotateX;
	bool mKeepRotateY;
	bool mKeepRotateZ;

	void rotateModel(glm::mat4 &model);
	void generateCube();
	void initializeCamera();
};

