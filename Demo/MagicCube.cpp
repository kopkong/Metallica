#include "StdAfx.h"
#include "MagicCube.h"
#include <gtc/matrix_transform.hpp>

const float CUBEMARGIN = 3.5f;

MagicCube::MagicCube(Camera* pCamera)
{
	mCamera = pCamera;
	mKeepRotateX = false;
	mKeepRotateY = false;
	mKeepRotateZ = false;
}

MagicCube::~MagicCube(void)
{
	
}

void MagicCube::init()
{
	generateCube();
	initializeCamera();
	
	// Create Program
	mProgName = glCreateProgram();
	compileShaderFromFile("../data/shader/cubemagic_vs.glsl",GL_VERTEX_SHADER,mProgName);
	compileShaderFromFile("../data/shader/cubemagic_fs.glsl",GL_FRAGMENT_SHADER,mProgName);

	// Find uniform location
	mUniformMVP = glGetUniformLocation(mProgName,"MVP");
	mUniformCubeTexture = glGetUniformLocation(mProgName,"Sampler2D");
	//mUniformTextureBufferOffset = glGetUniformLocation(mProgName,"InstancedOffset");

	// Load Textures
	mTextureCubeFace = loadTexture2D(L"../data/cubemagic/cube.DDS");
	glGenerateMipmap(GL_TEXTURE_2D);

	// Initialize vertex buffer
	glGenBuffers(1,&mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,mVertexCount * sizeof(vertex_v3fv2f) ,&mVertexData[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	// Initiliaz offest buffer
	//glGenTextures(1,&mInstancedVertexOffsetBuffer);
	//glBindBuffer(GL_TEXTURE_BUFFER,mInstancedVertexOffsetBuffer);
	//glBufferData(GL_TEXTURE_BUFFER,mInstanceCount * sizeof(glm::vec3), &mInstancedOffsetData[0],GL_STATIC_DRAW);
	//glBindBuffer(GL_TEXTURE_BUFFER,0);

	// Bind offset buffer to texture buffer object
	//glGenTextures(1,&mTextureInstanceOffsetBuffer);
	//glBindTexture(GL_TEXTURE_BUFFER,mTextureInstanceOffsetBuffer);
	//glTexBuffer(GL_TEXTURE_BUFFER,GL_RGB32F,mInstancedVertexOffsetBuffer);
	//glBindTexture(GL_TEXTURE_BUFFER,0);
}

void MagicCube::render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(mProgName);

	// Bind Vertex
	glBindBuffer(GL_ARRAY_BUFFER,mVertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_v3fv2f),0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex_v3fv2f),(const GLvoid*)sizeof(glm::vec3));

	// Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,mTextureCubeFace);
	glUniform1i(mUniformCubeTexture,0);

	// Texture buffer
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_BUFFER,mTextureInstanceOffsetBuffer);
	//glUniform1i(mUniformTextureBufferOffset,1);

	// Cull face
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	for(unsigned int i = 0 ; i < mInstanceCount ; i++)
	{
		rotateModel(mModelCoordinateData[i]);
		mCamera->setModel(mModelCoordinateData[i]);

		glm::mat4 mvp = mCamera->getMVP();

		glUniformMatrix4fv(mUniformMVP,1,GL_FALSE,&mvp[0][0]);

		glDrawArrays(GL_TRIANGLES,0,mVertexCount);
	}

	// Disable attrib pointer
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	checkError("cube render");
}

void MagicCube::update()
{

}

void MagicCube::exit()
{

}

void MagicCube::rotateModel(glm::mat4 &model)
{
	const glm::vec3 YAXIS = glm::vec3(0.0,1.0,0.0);
	const glm::vec3 XAXIS = glm::vec3(1.0,0.0,0.0);
	const glm::vec3 ZAXIS = glm::vec3(0.0,0.0,1.0);

	if(mKeepRotateX)
		model = glm::gtc::matrix_transform::rotate(model,1.0f,XAXIS);
	if(mKeepRotateY)
		model = glm::gtc::matrix_transform::rotate(model,1.0f,YAXIS);
	if(mKeepRotateZ)
		model = glm::gtc::matrix_transform::rotate(model,1.0f,ZAXIS);
}

void MagicCube::onKeyboard(SDL_Keycode Key)
{
	switch(Key)
	{
	case SDLK_x:
		mKeepRotateX = !mKeepRotateX;
		break;
	case SDLK_y:
		mKeepRotateY = !mKeepRotateY;
		break;
	case SDLK_z:
		mKeepRotateZ = !mKeepRotateZ;
		break;
	default:
		break;
	}
}

void MagicCube::generateCube()
{
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec2(0.000059f, 1.0f-0.000004f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f, 1.0f),glm::vec2(0.000103f, 1.0f-0.336048f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec2(0.335973f, 1.0f-0.335903f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f,-1.0f),glm::vec2(1.000023f, 1.0f-0.000013f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec2(0.667979f, 1.0f-0.335851f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f,-1.0f),glm::vec2(0.999958f, 1.0f-0.336064f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f, 1.0f),glm::vec2(0.667979f, 1.0f-0.335851f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec2(0.336024f, 1.0f-0.671877f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f,-1.0f),glm::vec2(0.667969f, 1.0f-0.671889f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f,-1.0f),glm::vec2(1.000023f, 1.0f-0.000013f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f,-1.0f),glm::vec2(0.668104f, 1.0f-0.000013f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec2(0.667979f, 1.0f-0.335851f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec2(0.000059f, 1.0f-0.000004f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec2(0.335973f, 1.0f-0.335903f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f,-1.0f),glm::vec2(0.336098f, 1.0f-0.000071f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f, 1.0f),glm::vec2(0.667979f, 1.0f-0.335851f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f, 1.0f),glm::vec2(0.335973f, 1.0f-0.335903f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f,-1.0f),glm::vec2(0.336024f, 1.0f-0.671877f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec2(1.000004f, 1.0f-0.671847f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f,-1.0f, 1.0f),glm::vec2(0.999958f, 1.0f-0.336064f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f, 1.0f),glm::vec2(0.667979f, 1.0f-0.335851f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f, 1.0f),glm::vec2(0.668104f, 1.0f-0.000013f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f,-1.0f),glm::vec2(0.335973f, 1.0f-0.335903f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f,-1.0f),glm::vec2(0.667979f, 1.0f-0.335851f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f,-1.0f),glm::vec2(0.335973f, 1.0f-0.335903f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f, 1.0f),glm::vec2(0.668104f, 1.0f-0.000013f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f, 1.0f),glm::vec2(0.336098f, 1.0f-0.000071f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f, 1.0f),glm::vec2(0.000103f, 1.0f-0.336048f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f,-1.0f),glm::vec2(0.000004f, 1.0f-0.671870f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f,-1.0f),glm::vec2(0.336024f, 1.0f-0.671877f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f, 1.0f),glm::vec2(0.000103f, 1.0f-0.336048f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f,-1.0f),glm::vec2(0.336024f, 1.0f-0.671877f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec2(0.335973f, 1.0f-0.335903f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f, 1.0f, 1.0f),glm::vec2(0.667969f, 1.0f-0.671889f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3(-1.0f, 1.0f, 1.0f),glm::vec2(1.000004f, 1.0f-0.671847f)));
	mVertexData.push_back(vertex_v3fv2f(glm::vec3( 1.0f,-1.0f, 1.0),glm::vec2(0.667979f, 1.0f-0.33585)));

	mVertexCount = mVertexData.size();

	// Generate instanced offset
	for(int y = 0 ; y < CUBEYROWS; y ++)
	{
		for(int z = 0 ; z <CUBEZROWS; z ++)
		{
			for(int x = 0; x < CUBEXROWS; x++)
			{
				glm::vec3 offset = glm::vec3(x*CUBEMARGIN,y*CUBEMARGIN,z*CUBEMARGIN);
				glm::mat4 model = glm::mat4(1.0);
				model = glm::gtc::matrix_transform::translate(model,offset);

				mModelCoordinateData.push_back(model);
			}
		}
	}

	mInstanceCount = mModelCoordinateData.size();
}

void MagicCube::initializeCamera()
{
	glm::vec3 targetPos = glm::vec3(CUBEXROWS,CUBEYROWS,CUBEZROWS);

	glm::vec3 cameraPos = glm::vec3(
		4 + targetPos.x * 1.5,
		5 + targetPos.y * 2,
	    4 + targetPos.z * 1.5
		);

	glm::vec3 up = glm::vec3(0.0,1.0,0.0);

	mCamera->resetCamera(cameraPos,targetPos,up);

}