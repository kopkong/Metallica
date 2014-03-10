#include "StdAfx.h"
#include "MagicCube.h"
#include <gtc/matrix_transform.hpp>

const float CUBEMARGIN = 3.5f;
const float EyesSight = 100.0f;
const float LaunchSpeed = 1.0f;

MagicCube::MagicCube(Camera* pCamera)
{
	mCamera = pCamera;
	mKeepRotateX = false;
	mKeepRotateY = false;
	mKeepRotateZ = false;

	mScreenSize = glm::ivec2(WINDOW_WIDTH,WINDOW_HEIGHT);
	mSelectedInstance = -1; // unselected
	mIsPicking = false;
	//mRayOrigin = glm::vec3(0,0,0);
	//mRayDirection = glm::vec3(1,100,10);
}

MagicCube::~MagicCube(void)
{
	
}

void MagicCube::init()
{
	generateCube();
	initializeCamera();
	
	// Create Program
	mProgCubeRendering = glCreateProgram();
	compileShaderFromFile("../data/shader/cubemagic_vs.glsl",GL_VERTEX_SHADER,mProgCubeRendering);
	compileShaderFromFile("../data/shader/cubemagic_fs.glsl",GL_FRAGMENT_SHADER,mProgCubeRendering);

	mProgRayTest = glCreateProgram();
	compileShaderFromFile("../data/shader/raytest.vertexshader",GL_VERTEX_SHADER,mProgRayTest);
	compileShaderFromFile("../data/shader/raytest.fragmentshader",GL_FRAGMENT_SHADER,mProgRayTest);

	// Find uniform location
	mUniformMVP = glGetUniformLocation(mProgCubeRendering,"MVP");
	mUniformCubeTexture = glGetUniformLocation(mProgCubeRendering,"Sampler2D");
	mUniformRayTestMVP = glGetUniformLocation(mProgRayTest,"MVP");
	//mUniformTextureBufferOffset = glGetUniformLocation(mProgCubeRendering,"InstancedOffset");

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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	//renderCube();

	renderRollingCube();
}

void MagicCube::renderCube()
{
	glUseProgram(mProgCubeRendering);

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

	// Print Camera position
	glm::vec3 cPos = mCamera->getPos();
	glm::vec3 cDir = mCamera->getLookatDirection();
	SDL_Log("Camera pos : {%f,%f,%f}",cPos.x,cPos.y,cPos.z);
	SDL_Log("Camera look at position :{%f,%f,%f}",cDir.x,cDir.y,cDir.z);

	// Cull face
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

void MagicCube::renderRollingCube()
{
	glUseProgram(mProgCubeRendering);

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

	// Cull face
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

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

void MagicCube::renderRayTest()
{
	glUseProgram(mProgRayTest);

	GLuint vao;
	glGenVertexArrays(1,&vao);
	glBindVertexArray(vao);

	GLuint rayVertexBuffer;
	glGenBuffers(1,&rayVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,rayVertexBuffer);

	glm::vec3 ray[2] = {
		mCamera->getPos(),
		mCamera->getLookatDirection() * EyesSight
		//rayStart,
		//rayEnd 
	};
	glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3) * 2,&ray[0],GL_STATIC_DRAW);

	SDL_Log("Ray origin is (%f,%f,%f)",mRayOrigin.x,mRayOrigin.y,mRayOrigin.z);
	SDL_Log("Ray direction is  (%f,%f,%f)",mRayDirection.x,mRayDirection.y,mRayDirection.z);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),0);

	mCamera->setModel(glm::mat4(1.0));
	glm::mat4 mvp = mCamera->getMVP();
	glUniformMatrix4fv(mUniformRayTestMVP,1,GL_FALSE,&mvp[0][0]);

	glPointSize(10.0);
	glLineWidth(2.0);
	
	// draw end
	glDrawArrays(GL_POINTS,1,1);

	// draw ray
	glDrawArrays(GL_LINES,0,2);

	glDisableVertexAttribArray(0);
}

void MagicCube::update()
{
	// Update the rolling cube's position
	mInstanceCount = mModelCoordinateData.size();

	// Picking cube
	//if(mIsPicking && (mMousePos.x > 0 && mMousePos.y > 0))
	//{
	//	//SDL_Log("Picking .. MousePosition is (%d,%d)",mMousePos.x, mMousePos.y);
	//	glm::mat4 viewMatrix = mCamera->getView();
	//	glm::mat4 projMatrix = mCamera->getProjection();
	//	glm::vec3 ray_Origin;
	//	glm::vec3 ray_Direction;
	//	screenPosToWorldRay(
	//		mMousePos.x / 2 , mMousePos.y / 2,
	//		mScreenSize.x, mScreenSize.y,
	//		viewMatrix,
	//		projMatrix,
	//		ray_Origin,
	//		ray_Direction
	//		);
	//	mRayOrigin = ray_Origin;
	//	mRayDirection = ray_Direction;

	//	float intersectionDistance; // Output of TestRayOBBIntersection()
	//	glm::vec3 aabb_min(-1.0f, -1.0f, -1.0f);
	//	glm::vec3 aabb_max( 1.0f,  1.0f,  1.0f);

	//	for(unsigned i = 0 ; i < mInstanceCount ; i++)
	//	{
	//		if(testRayOBBIntersection(ray_Origin,ray_Direction,aabb_min,aabb_max,
	//			mModelCoordinateData[i],intersectionDistance))
	//		{
	//			mSelectedInstance = i;
	//			SDL_Log("The %d Cube is selected ",mSelectedInstance);
	//		}
	//	}

	//	//remove picking state
	//	SDL_Log("No cube is selected");
	//	mIsPicking = false;
	//}
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

}

void MagicCube::initializeCamera()
{
	glm::vec3 cameraPos = glm::vec3(CUBEMARGIN * CUBEXROWS / 2,CUBEMARGIN * CUBEYROWS / 2, -30);

	mCamera->setCameraPosition(cameraPos);
}

void MagicCube::screenPosToWorldRay(int mouseX,int mouseY,int screenWidth,int screenHeight,
		glm::mat4 viewMatrix,glm::mat4 projMatrix,glm::vec3& out_origin,glm::vec3& out_direction)
{
	//glm::vec4 lRayStart_NDC( 
	//	(mouseX/(float)screenWidth - 0.5f) * 2.0,
	//	(mouseY/(float)screenHeight - 0.5f) * 2.0,
	//	-1.0,
	//	1.0f
	//	);

	glm::vec4 lRayEnd_NDC(
		(mouseX/(float)screenWidth - 0.5f) * 2.0,
		(mouseY/(float)screenHeight - 0.5f) * 2.0,
		0.0,
		1.0f
		);

	glm::vec4 lRayStart_NDC(mCamera->getPos(),1.0f);

	glm::mat4 inverseProjMatrix = glm::inverse(projMatrix);
	glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);

	glm::vec4 lRayStart_World	   = lRayStart_NDC; 
	glm::vec4 lRayEnd_View   = inverseProjMatrix * lRayEnd_NDC; lRayEnd_View /= lRayEnd_View.w;
	glm::vec4 lRayEnd_World  = inverseViewMatrix * lRayEnd_View; lRayEnd_World /= lRayEnd_World.w;

	glm::vec3 lRayDirection_World(lRayEnd_World - lRayStart_World);
	lRayDirection_World = glm::normalize(lRayDirection_World);

	out_origin = glm::vec3(lRayStart_World);
	out_direction = glm::vec3(lRayDirection_World);
}

bool MagicCube::testRayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction,glm::vec3 aabb_min,
		glm::vec3 aabb_max, glm::mat4 modelMatrix,float& intersectionDistance)
{
	float tMin = 0.0f;
	float tMax = 1000000.0f;

	glm::vec3 obbWorldSpace(modelMatrix[3].x,modelMatrix[3].y,modelMatrix[3].z);
	glm::vec3 delta = obbWorldSpace - ray_origin;

	// Test intersection with the 2 planes perpendicular  to the OBB's X axis
	{
		glm::vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
		float e = glm::dot(xAxis,delta);
		float f = glm::dot(delta, xAxis);

		if( fabs(f) > 0.001f)
		{
			float t1 = (e + aabb_min.x) / f;
			float t2 = (e + aabb_max.x) / f;

			// make sure t1 is smaller than t2
			if(t1 > t2)
			{
				engineReverse(t1,t2);
			}

			if( t2 < tMax)
				tMax = t2;
			if( t1 > tMin)
				tMin = t1;

			if(tMax < tMin)
				return false;
		}
		else
		{ // the ray is parallel to the planes
			if( aabb_min.x - e > 0.0f || aabb_max.x - e < 0.0f)
				return false;
		}

	}

	// Test intersection with the 2 planes perpendicular  to the OBB's Y axis
	{
		glm::vec3 yAxis(modelMatrix[1].x,modelMatrix[1].x,modelMatrix[1].z);
		float e = glm::dot(yAxis,delta);
		float f = glm::dot(ray_direction, yAxis);

		if(fabs(f) > 0.001f)
		{
			float t1 = (e + aabb_min.x) / f;
			float t2 = (e + aabb_max.x) / f;

			// make sure t1 is smaller than t2
			if(t1 > t2)
			{
				engineReverse(t1,t2);
			}

			if( t2 < tMax)
				tMax = t2;
			if( t1 > tMin)
				tMin = t1;

			if(tMax < tMin)
				return false;
		}
		else
		{
			if( aabb_min.y - e > 0.0f || aabb_max.y -e < 0.0f)
				return false;
		}
	}

	
	// Test intersection with the 2 planes perpendicular  to the OBB's Z axis
	{
		glm::vec3 zAxis(modelMatrix[2].x,modelMatrix[2].x,modelMatrix[2].z);
		float e = glm::dot(zAxis,delta);
		float f = glm::dot(ray_direction, zAxis);

		if(fabs(f) > 0.001f)
		{
			float t1 = (e + aabb_min.x) / f;
			float t2 = (e + aabb_max.x) / f;

			// make sure t1 is smaller than t2
			if(t1 > t2)
			{
				engineReverse(t1,t2);
			}

			if( t2 < tMax)
				tMax = t2;
			if( t1 > tMin)
				tMin = t1;

			if(tMax < tMin)
				return false;
		}
		else
		{
			if( aabb_min.z - e > 0.0f || aabb_max.z -e < 0.0f)
				return false;
		}
	}

	intersectionDistance = tMin;
	return true;
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

void MagicCube::onMouseDown(SDL_MouseButtonEvent button)
{
	if(button.button == SDL_BUTTON_LEFT && button.state== SDL_PRESSED)
	{
		// Launch a cube
		MotionCube cube(mCamera->getPos(),mCamera->getLookatDirection(),LaunchSpeed,0.0f);
		mMotionCubes.push_back(cube);
	}
}