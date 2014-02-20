#include "TerrainRender.h"
#include "gtc/matrix_transform.hpp"

const char* TERRAIN_RESOURCES_DIR = "../data/terrain/";
char* SHADER_DIR = "../data/shader/";

TerrainRender::TerrainRender(Camera* pCamera)
{
	mInitOK = false;
	mCamera = pCamera;
	mProgName = 0;
	mFrameCount = 0;
}

TerrainRender::~TerrainRender(void)
{
	exit();
}

void TerrainRender::init()
{
	initializeTerrain();
	initializeCamera();

	mProgName = glCreateProgram();
	compileShaderFromFile("../data/shader/terrain_vs.glsl",GL_VERTEX_SHADER,mProgName);
	compileShaderFromFile("../data/shader/terrain_fs.glsl",GL_FRAGMENT_SHADER,mProgName);

	mUniformTerrainSampler = glGetUniformLocation(mProgName,"gSampler");
	mUniformHeightMap = glGetUniformLocation(mProgName,"HeightMap");
	mUniformMVP = glGetUniformLocation(mProgName,"MVP");

	// load terrain texture
	mTerrainTexture = loadTexture2D(L"../data/terrain/green.bmp");

	// load height map
	mHeightMap = loadTexture2D(L"../data/terrain/Terrain.bmp");

	glGenBuffers(1,&mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);	
	glBufferData(GL_ARRAY_BUFFER,mVertexDataSize,mVertexData,GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	//glGenBuffers(1,&mElementBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mElementBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,mVertexElementDataSize,mVertexElementData,GL_STATIC_DRAW);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	checkError("After Init");
}

void TerrainRender::render()
{
	mFrameCount ++;

	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(mProgName);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(vertex_v2fv2f),0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex_v2fv2f),(const void*)sizeof(glm::vec2));

	// Bind Terrain texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,mTerrainTexture);
	glUniform1i(mUniformTerrainSampler,0);

	// Bind Heightmap
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,mHeightMap);
	glUniform1i(mUniformHeightMap,1);

	// Set MVP
	glm::mat4 mvp = mCamera->getMVP();
	glUniformMatrix4fv(mUniformMVP,1,GL_FALSE,&mvp[0][0]);

	glDrawArrays(GL_TRIANGLES,0,6 * ( MAP_X -1 ) * (MAP_Z - 1));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	//SDL_Log("Render the %d frame",mFrameCount);
	//renderTest();

	checkError("Terrain Render");
}

void TerrainRender::exit()
{
	glDeleteBuffers(1,&mVertexBuffer);
	glDeleteBuffers(1,&mElementBuffer);
	glDeleteProgram(mProgName);
}

void TerrainRender::initializeTerrain()
{	
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

			// vertex one
			vertex_v2fv2f vOne = {glm::vec2(x,z),glm::vec2(0.0,0.0)};

			// vertex two
			vertex_v2fv2f vTwo = {glm::vec2(x,z + 1),glm::vec2(1.0,0.0)};

			// vetex three 
			vertex_v2fv2f vThree = {glm::vec2(x +1 ,z ),glm::vec2(1.0,1.0)};

			// vetex four , repeat the third
			vertex_v2fv2f vFour = vThree;

			// vetex five 
			vertex_v2fv2f vFive = {glm::vec2(x +1,z + 1),glm::vec2(0.0,1.0)};

			// vetex six , repeat the second
			vertex_v2fv2f vSix = vTwo;

			mVertexData[idx] = vOne;
			mVertexData[idx + 1] = vTwo;
			mVertexData[idx + 2] = vThree;
			mVertexData[idx + 3] = vFour;
			mVertexData[idx + 4] = vFive;
			mVertexData[idx + 5] = vSix;
		}
	}

	mVertexDataSize = sizeof(mVertexData);
	//mVertexElementDataSize = sizeof(mVertexElementData);
	//mElementCount =  mVertexElementDataSize/sizeof(unsigned int) ;

}

void TerrainRender::initializeCamera()
{
	glm::vec3 targetPos = glm::vec3(
		MAP_X * MAP_SCALE /2.0 ,
		100.0f,
		MAP_Z * MAP_SCALE /2.0
	);

	glm::vec3 cameraPos = glm::vec3(
		MAP_X * MAP_SCALE ,
		256 * 1.5 ,
		MAP_Z * MAP_SCALE 
		);

	glm::vec3 up = glm::vec3(0.0,1.0,0.0);

	mCamera->resetCamera(cameraPos,targetPos,up);

}

void TerrainRender::initializeTest()
{
	//initializeTerrain();

	mTestProgram = glCreateProgram();
	compileShaderFromFile("../data/shader/test.vertexshader",GL_VERTEX_SHADER,mTestProgram);
	compileShaderFromFile("../data/shader/test.fragmentshader",GL_FRAGMENT_SHADER,mTestProgram);

	// Get a handle for our "MVP" uniform
	mTestUniformMVP = glGetUniformLocation(mTestProgram, "MVP");

	mCamera->resetCamera(glm::vec3(4,3,-3),glm::vec3(0,0,0),glm::vec3(0,1,0));

	static const GLfloat g_vertex_buffer_data[] = { 
		0.0f,1.0f,0.0f,
		1.0f,1.0f,0.0f,
		1.0f,1.0f, 1.0f
	};

	static const GLfloat g_color_buffer_data[] = { 
		0.583f,  0.771f,  0.014f,
		0.609f,  0.115f,  0.436f,
		0.327f,  0.483f,  0.844f,
	};

	//GLuint vertexbuffer;
	glGenBuffers(1, &mTestVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTestVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//GLuint colorbuffer;
	glGenBuffers(1, &mTestColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mTestColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
}

void TerrainRender::renderTest()
{
	// Use our shader
	glUseProgram(mTestProgram);

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glm::mat4 MVP = mCamera->getMVP();
	glUniformMatrix4fv(mTestUniformMVP, 1, GL_FALSE, &MVP[0][0]);

	// 1rst attribute buffer : vertices
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mTestVertexBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
	);

	// 2nd attribute buffer : colors
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mTestColorBuffer);
	glVertexAttribPointer(
		1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
		3,                                // size
		GL_FLOAT,                         // type
		GL_FALSE,                         // normalized?
		0,                                // stride
		(void*)0                          // array buffer offset
	);

	// Draw the triangle !
	glDrawArrays(GL_TRIANGLES, 0, 3); // 12*3 indices starting at 0 -> 12 triangles

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}