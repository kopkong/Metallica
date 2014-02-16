#pragma once
#include "StdAfx.h"
#include "camera.h"

#define MAP_X	3				         // size of map along x-axis
#define MAP_Z	3				         // size of map along z-axis
#define MAP_SCALE	200.0f		         // the scale of the terrain map

class TerrainRender
{
public:
	TerrainRender(Camera*);
	~TerrainRender(void);
	void init();
	void render();

	bool mInitOK;
	void exit();

private:
	GLuint mProgName;
	Camera *mCamera;
	GLuint mUniformTerrainSampler;
	GLuint mUniformHeightMap;
	GLuint mTerrainTexture;
	GLuint mHeightMap;
	GLuint mVertexBuffer;
	GLuint mElementBuffer;
	vertex_v2fv2f mVertexData[MAP_X * MAP_Z];
	unsigned int mVertexElementData[6 * ( MAP_X -1 ) * (MAP_Z - 1)];
	int mVertexDataSize;
	int mElementCount;
	int mVertexElementDataSize;


	void initializeTerrain();
};

