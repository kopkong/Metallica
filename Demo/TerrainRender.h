#pragma once
#include "StdAfx.h"
#include "camera.h"
#include "engine.inl"

#define MAP_X	32				         // size of map along x-axis
#define MAP_Z	32				         // size of map along z-axis
#define MAP_SCALE	10.0f		         // the scale of the terrain map

class TerrainRender
{
public:
	TerrainRender(Camera*);
	~TerrainRender(void);
	void init();
	void render();

	bool mInitOK;
	void exit();

	void keyBoardHandler(SDL_Event*);
	Camera *mCamera;
private:
	int mFrameCount;

	GLuint mProgName;
	GLuint mUniformTerrainSampler;
	GLuint mUniformHeightMap;
	GLuint mUniformMVP;
	
	GLuint mTerrainTexture;
	GLuint mHeightMap;
	GLuint mVertexBuffer;
	GLuint mElementBuffer;
	vertex_v2fv2f mVertexData[6 * ( MAP_X -1 ) * (MAP_Z - 1)];
	unsigned int mVertexElementData[6 * ( MAP_X -1 ) * (MAP_Z - 1)];
	int mVertexDataSize;
	int mElementCount;
	int mVertexElementDataSize;

	void initializeTerrain();
	void initializeCamera();
	
	// For test
	GLuint mTestVertexBuffer;
	GLuint mTestColorBuffer;
	GLuint mTestProgram;
	GLuint mTestUniformMVP;
	void initializeTest();
	void renderTest();
};

