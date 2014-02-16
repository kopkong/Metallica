#include "TerrainRender.h"

const char* TERRAIN_RESOURCES_DIR = "../data/terrain/";
char* SHADER_DIR = "../data/shader/";

TerrainRender::TerrainRender(Camera* pCamera)
{
	mInitOK = false;
	mCamera = pCamera;
	mProgName = 0;
}

TerrainRender::~TerrainRender(void)
{
	exit();
}

void TerrainRender::init()
{
	initializeTerrain();

	mProgName = glCreateProgram();
	compileShaderFromFile("../data/shader/terrain_vs.glsl",GL_VERTEX_SHADER,mProgName);
	compileShaderFromFile("../data/shader/terrain_fs.glsl",GL_FRAGMENT_SHADER,mProgName);

	mUniformTerrainSampler = glGetUniformLocation(mProgName,"gSampler");

	glGenTextures(1,&mTerrainTexture);
	if(!loadTexture2D(L"../data/terrain/green.bmp",mTerrainTexture))
	{
		SDL_Log("Load terrain texture fail!");
	}
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	glGenBuffers(1,&mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);	
	glBufferData(GL_ARRAY_BUFFER,mVertexDataSize,mVertexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glGenBuffers(1,&mElementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mElementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,mVertexElementDataSize,mVertexElementData,GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	checkError("After Init");
}

void TerrainRender::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(mProgName);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(vertex_v2fv2f),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex_v2fv2f),(const void*)sizeof(glm::vec2));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,mTerrainTexture);
	glUniform1i(mUniformTerrainSampler,0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mElementBuffer);
	glDrawElements(GL_TRIANGLES,mElementCount,GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	checkError("Terrain Render");
}

void TerrainRender::exit()
{
	glDeleteBuffers(1,&mVertexBuffer);
	glDeleteProgram(mProgName);
}

void TerrainRender::initializeTerrain()
{
	glm::vec2 textureCoordinates[4]=
	{
			glm::vec2(0.0,0.0),
			glm::vec2(1.0,0.0),
			glm::vec2(1.0,1.0),
			glm::vec2(0.0,1.0)
	};

	glm::vec2 terrainPoints[MAP_X][MAP_Z];
	int coordIndex = 0;
	
	// Generate vertex position and texture coordinate
	for(int z =0; z< MAP_Z; z++)
	{
		for(int x=0; x< MAP_X;x++)
		{
			float fX = float(x) * MAP_SCALE;
			float fZ = float(z) * MAP_SCALE;
			if(coordIndex ==4)
				coordIndex = 0;

			vertex_v2fv2f vertexInfo =  {glm::vec2(fX,fZ),textureCoordinates[coordIndex]};
			mVertexData[x + z*MAP_Z] = vertexInfo;
			coordIndex ++;
		}
	}
	
	/* Generate element array */
	 
	int slimMap_Z = MAP_Z - 1;
	int slimMap_X = MAP_X - 1;
	// We have to use MAP_X - 1 and MAP_Z - 1,
	// because we can't use the last x/z axis as the starter vertex of any triangle
	
	for(int z = 0; z< slimMap_Z ; z++)
	{
		for(int x =0; x < slimMap_X ;x++)
		{
			int idx =  x * 6 + z * slimMap_X * 6;

			// These six vertex could draw one square
			// Draw order
			//
			//  v3---------v4
			//   |				 |
			//   |				 |
			//  v1---------v2
			//  first triangle = v1,v2,v3  
			//  second triangle = v3,v4,v2
			mVertexElementData[idx] = x + z*MAP_Z; 
			mVertexElementData[idx + 1] = x + (z+1)*MAP_Z;
			mVertexElementData[idx + 2] = (x + 1) + z*MAP_Z;
			mVertexElementData[idx + 3] = (x + 1) + z*MAP_Z;      // repeat the third	 
			mVertexElementData[idx + 4] =  (x + 1) + (z +1)*MAP_Z ;
			mVertexElementData[idx + 5] =  x + (z+1)*MAP_Z;        // repeat the second
		}
	}

	mVertexDataSize = sizeof(mVertexData);
	mVertexElementDataSize = sizeof(mVertexElementData);
	mElementCount =  mVertexElementDataSize/sizeof(unsigned int) ;

}
