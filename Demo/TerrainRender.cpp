#include "TerrainRender.h"
#include "gtc/matrix_transform.hpp"

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
	initializeCamera();

	mProgName = glCreateProgram();
	compileShaderFromFile("../data/shader/terrain_vs.glsl",GL_VERTEX_SHADER,mProgName);
	compileShaderFromFile("../data/shader/terrain_fs.glsl",GL_FRAGMENT_SHADER,mProgName);

	mUniformTerrainSampler = glGetUniformLocation(mProgName,"gSampler");
	mUniformMVP = glGetUniformLocation(mProgName,"MVP");

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

	//initializeTest();

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

	// Bind Terrain texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,mTerrainTexture);
	glUniform1i(mUniformTerrainSampler,0);

	// Set MVP
	glm::mat4 mvp = mCamera->getMVP();
	glUniformMatrix4fv(mUniformMVP,1,GL_FALSE,&mvp[0][0]);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mElementBuffer);
	glDrawElements(GL_TRIANGLES,mElementCount,GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

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

void TerrainRender::initializeCamera()
{
	glm::vec3 targetPos = glm::vec3(
		50,0,50
	);

	glm::vec3 cameraPos = glm::vec3(
		150,100,150
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