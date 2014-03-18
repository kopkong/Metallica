#include "StdAfx.h"
#include "MagicCube.h"
#include <gtc/matrix_transform.hpp>

const float CUBEMARGIN = 3.5f;
const float EyesSight = 100.0f;
const float LaunchSpeed = 1.0f;
const float TessLevelInner = 3.0f;
const float TessLevelOuter = 2.0f;
static int cubeCount = 0;

MagicCube::MagicCube(Camera* pCamera)
{
	mCamera = pCamera;

	mScreenSize = glm::ivec2(WINDOW_WIDTH,WINDOW_HEIGHT);
	mSelectedInstance = -1; // unselected
	mIsPicking = false;
	mSkyBoxCoordinateData = glm::mat4(1.0);
}

MagicCube::~MagicCube(void)
{
	
}

void MagicCube::init()
{
	generateCube();
	initializeCamera();
	
	// Create Program
	mCubeProg.ProgramId = glCreateProgram();
	compileShaderFromFile("../data/shader/cubemagic_vs.glsl",GL_VERTEX_SHADER,mCubeProg.ProgramId);
	compileShaderFromFile("../data/shader/cubemagic_fs.glsl",GL_FRAGMENT_SHADER,mCubeProg.ProgramId);

	mTestProg.ProgramId = glCreateProgram();
	compileShaderFromFile("../data/shader/raytest.vertexshader",GL_VERTEX_SHADER,mTestProg.ProgramId);
	compileShaderFromFile("../data/shader/raytest.fragmentshader",GL_FRAGMENT_SHADER,mTestProg.ProgramId);

	mProgSkyBox = glCreateProgram();
	compileShaderFromFile("../data/shader/skybox.vert",GL_VERTEX_SHADER,mProgSkyBox);
	compileShaderFromFile("../data/shader/skybox.frag",GL_FRAGMENT_SHADER,mProgSkyBox);

	mTessSphereProg.ProgramId = glCreateProgram();
	compileShaderFromFile("../data/shader/tess.vert",GL_VERTEX_SHADER,mTessSphereProg.ProgramId);
	compileShaderFromFile("../data/shader/tess.con",GL_TESS_CONTROL_SHADER,mTessSphereProg.ProgramId);
	compileShaderFromFile("../data/shader/tess.eva",GL_TESS_EVALUATION_SHADER,mTessSphereProg.ProgramId);
	compileShaderFromFile("../data/shader/tess.gemo",GL_GEOMETRY_SHADER,mTessSphereProg.ProgramId);
	compileShaderFromFile("../data/shader/tess.frag",GL_FRAGMENT_SHADER,mTessSphereProg.ProgramId);

	mCrossHairProg.ProgramId = glCreateProgram();
	compileShaderFromFile("../data/shader/crosshair.vert",GL_VERTEX_SHADER,mCrossHairProg.ProgramId);
	compileShaderFromFile("../data/shader/crosshair.frag",GL_FRAGMENT_SHADER,mCrossHairProg.ProgramId);

	// Find uniform location
	{
		mCubeProg.UniformLocations.MVP = glGetUniformLocation(mCubeProg.ProgramId,"MVP");
		mCubeProg.UniformLocations.Texture = glGetUniformLocation(mCubeProg.ProgramId,"Sampler2D");

		mUniformCameraPosition = glGetUniformLocation(mProgSkyBox,"CameraPosition");
		mUniformViewProjectMatrix = glGetUniformLocation(mProgSkyBox,"ViewProjectionMatrix");
		mUniformSkyBoxTexture = glGetUniformLocation(mProgSkyBox,"CubeMap");

		mTestProg.UniformLocations.MVP = glGetUniformLocation(mTestProg.ProgramId,"MVP");

		mTessSphereProg.UniformLocations.TessLevelInner = glGetUniformLocation(mTessSphereProg.ProgramId,"TessLevelInner");
		mTessSphereProg.UniformLocations.TessLevelOuter = glGetUniformLocation(mTessSphereProg.ProgramId,"TessLevelOuter");
		mTessSphereProg.UniformLocations.Projection = glGetUniformLocation(mTessSphereProg.ProgramId,"Projection");
		mTessSphereProg.UniformLocations.ModelView = glGetUniformLocation(mTessSphereProg.ProgramId,"ModelView");
		mTessSphereProg.UniformLocations.NormalMatrix = glGetUniformLocation(mTessSphereProg.ProgramId,"NormalMatrix");

		mCrossHairProg.UniformLocations.Color = glGetUniformLocation(mCrossHairProg.ProgramId,"CrossHairColor");
	}

	// Load Textures
	{
		mCubeProg.TextureWood = loadTexture2D(L"../data/cubemagic/wood.jpg");
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);

		mCubeProg.TexturePlastic = loadTexture2D(L"../data/cubemagic/plastic.jpg");
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);

		mCubeProg.TextureMetal = loadTexture2D(L"../data/cubemagic/metal.jpg");
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	// Load Cube map for skybox
	{
		wchar_t const* cubemaps[6] = {
			L"../data/cubemagic/right.jpg",L"../data/cubemagic/left.jpg",
			L"../data/cubemagic/bottom.jpg",L"../data/cubemagic/top.jpg",
			L"../data/cubemagic/front.jpg",L"../data/cubemagic/back.jpg"
		};

		mCubeMapSkyBox = loadTextureCubeMap(&cubemaps[0]);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	}

	// Initialize cube vertex buffer
	{
		glGenBuffers(1,&mCubeProg.VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,mCubeProg.VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,mCubeProg.VertexCount * sizeof(vertex_v3fv2f) ,&mVertexData[0],GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}

	// Initialize buffer for tess sphere
	const float Verts[] = 
	{
         0.000f,  0.000f,  1.000f,
         0.894f,  0.000f,  0.447f,
         0.276f,  0.851f,  0.447f,
        -0.724f,  0.526f,  0.447f,
        -0.724f, -0.526f,  0.447f,
         0.276f, -0.851f,  0.447f,
         0.724f,  0.526f, -0.447f,
        -0.276f,  0.851f, -0.447f,
        -0.894f,  0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
         0.724f, -0.526f, -0.447f,
         0.000f,  0.000f, -1.000f
	};

	const int Faces[] = {
        2, 1, 0,
        3, 2, 0,
        4, 3, 0,
        5, 4, 0,
        1, 5, 0,

        11, 6,  7,
        11, 7,  8,
        11, 8,  9,
        11, 9,  10,
        11, 10, 6,

        1, 2, 6,
        2, 3, 7,
        3, 4, 8,
        4, 5, 9,
        5, 1, 10,

        2,  7, 6,
        3,  8, 7,
        4,  9, 8,
        5, 10, 9,
        1, 6, 10 };

	//unsigned int indexCount = sizeof(Faces) / sizeof(Faces[0]);
	mTessSphereProg.IndexCount = sizeof(Faces) / sizeof(Faces[0]);
	mTestProg.IndexCount = sizeof(Faces) / sizeof(Faces[0]);

	// Tessellataion sphere
	{
		glGenVertexArrays(1,&mTessSphereProg.VAO);
		glBindVertexArray(mTessSphereProg.VAO);

		glGenBuffers(1,&mTessSphereProg.VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,mTessSphereProg.VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(Verts),Verts,GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);

		glGenBuffers(1,&mTessSphereProg.ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mTessSphereProg.ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Faces),Faces,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		glBindVertexArray(0);
	}

	// Ray Test
	{
		glGenVertexArrays(1,&mTestProg.VAO);
		glBindVertexArray(mTestProg.VAO);

		glGenBuffers(1,&mTestProg.VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,mTestProg.VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(Verts),Verts,GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);

		glGenBuffers(1,&mTestProg.ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mTestProg.ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(Faces),Faces,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

		glBindVertexArray(0);
	}

	// Corss Hair
	{
		float rangeX = 50.0f / mScreenSize.x;
		float rangeY = 50.0f / mScreenSize.y;
		float crossHairPos[8] = 
		{
			rangeX,0.0,
			-rangeX,0.0,
			0.0,rangeY,
			0.0,-rangeY
		};

		glGenVertexArrays(1,&mCrossHairProg.VAO);
		glBindVertexArray(mCrossHairProg.VAO);

		glGenBuffers(1,&mCrossHairProg.VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,mCrossHairProg.VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,sizeof(crossHairPos),crossHairPos,GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindVertexArray(0);

		mCrossHairProg.VertexCount = sizeof(crossHairPos)/ sizeof(glm::vec2);
		mCrossHairProg.LineSize = 1.0f;
	}
}

void MagicCube::render()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	renderSkyBox();

	renderRollingCube();

	renderCrossHair();

	//renderSphere();

	//renderRayTest();
}

void MagicCube::renderSkyBox()
{
	glUseProgram(mProgSkyBox);

	// Bind vertex
	glBindBuffer(GL_ARRAY_BUFFER,mCubeProg.VertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_v3fv2f),0);

	// Bind Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,mCubeMapSkyBox);
	glUniform1i(mUniformSkyBoxTexture,0);
	
	// Translate the skybox to the camera position
	mCamera->setModel(mSkyBoxCoordinateData);
	glm::mat4 mvp = mCamera->getMVP();

	glUniform3fv(mUniformCameraPosition,1,&mCamera->getPos()[0]);
	glUniformMatrix4fv(mUniformViewProjectMatrix,1,GL_FALSE,&mvp[0][0]);

	glDrawArrays(GL_TRIANGLES,0,mCubeProg.VertexCount);

	glDisableVertexAttribArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,0);
	glUseProgram(0);

	checkError("skybox render");
}

void MagicCube::renderRollingCube()
{
	glUseProgram(mCubeProg.ProgramId);

	// Bind Vertex
	glBindBuffer(GL_ARRAY_BUFFER,mCubeProg.VertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(vertex_v3fv2f),0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,sizeof(vertex_v3fv2f),(const GLvoid*)sizeof(glm::vec3));

	// Cull face
	//glFrontFace(GL_CCW);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	// Depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	for(unsigned int i = 0 ; i < mCubeProg.InstanceCount ; i++)
	{
		// Set MVP Uniform
		mCamera->setModel(mModelCoordinateData[i]);
		glm::mat4 mvp = mCamera->getMVP();
		glUniformMatrix4fv(mCubeProg.UniformLocations.MVP,1,GL_FALSE,&mvp[0][0]);

		// Bind Texture
		glActiveTexture(GL_TEXTURE0);
		if(mModelMaterial[i] == Plastic)
			glBindTexture(GL_TEXTURE_2D,mCubeProg.TexturePlastic);
		if(mModelMaterial[i] == Wood)
			glBindTexture(GL_TEXTURE_2D,mCubeProg.TextureWood);
		if(mModelMaterial[i] == Metal)
			glBindTexture(GL_TEXTURE_2D,mCubeProg.TextureMetal);

		glUniform1i(mCubeProg.UniformLocations.Texture,0);
		glDrawArrays(GL_TRIANGLES,0,mCubeProg.VertexCount);
		glBindTexture(GL_TEXTURE_2D,0);
	}

	glDisable(GL_DEPTH_TEST);

	// Disable attrib pointer
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	checkError("cube render");
}

void MagicCube::renderRayTest()
{
	glUseProgram(mTestProg.ProgramId);

	glBindVertexArray(mTestProg.VAO);
	glBindBuffer(GL_ARRAY_BUFFER,mTestProg.VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mTestProg.ElementBuffer);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),0);

	mCamera->setModel(glm::mat4(1.0));
	glm::mat4 mvp = mCamera->getMVP();
	glUniformMatrix4fv(mTestProg.UniformLocations.MVP,1,GL_FALSE,&mvp[0][0]);
	
	glDrawElements(GL_TRIANGLES,mTestProg.IndexCount,GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(0);

	checkError("RayTest");
}

void MagicCube::renderSphere()
{
	glUseProgram(mTessSphereProg.ProgramId);

	glUniform1f(mTessSphereProg.UniformLocations.TessLevelInner,TessLevelInner);
	glUniform1f(mTessSphereProg.UniformLocations.TessLevelOuter,TessLevelOuter);

	glm::mat4 model(1.0);
	mCamera->setModel(model);

	glUniformMatrix4fv(mTessSphereProg.UniformLocations.Projection,1,GL_FALSE,&mCamera->getProjection()[0][0]);
	glUniformMatrix4fv(mTessSphereProg.UniformLocations.ModelView,1,GL_FALSE,&model[0][0]);
	float normalMatrix[9] = 
	{
		model[0][0],model[1][0],model[2][0],
		model[0][1],model[1][1],model[2][1],
		model[0][2],model[1][2],model[2][2]
	};
	glUniformMatrix3fv(mTessSphereProg.UniformLocations.NormalMatrix,1,GL_FALSE,&normalMatrix[0]);

	glBindVertexArray(mTessSphereProg.VAO);
	glBindBuffer(GL_ARRAY_BUFFER,mTessSphereProg.VertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,mTessSphereProg.ElementBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(glm::vec3),0);
	glPatchParameteri(GL_PATCH_VERTICES,3);

	glDrawElements(GL_PATCHES,mTessSphereProg.IndexCount,GL_UNSIGNED_INT,0);
	glDisableVertexAttribArray(0);

	checkError("renderSphere");
}

void MagicCube::renderCrossHair()
{
	glUseProgram(mCrossHairProg.ProgramId);

	glm::vec3 color(1.0,0.0,0.0);
	glUniform3fv(mCrossHairProg.UniformLocations.Color,1,&color[0]);

	glBindVertexArray(mCrossHairProg.VAO);
	glBindBuffer(GL_ARRAY_BUFFER,mCrossHairProg.VertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(glm::vec2),0);

	glLineWidth(mCrossHairProg.LineSize);
	glDrawArrays(GL_LINES,0,mCrossHairProg.VertexCount);

	glDisableVertexAttribArray(0);
}

void MagicCube::update()
{
	// Update skybox coordinate
	mSkyBoxCoordinateData = glm::translate(glm::mat4(1.0),mCamera->getPos());

	mModelCoordinateData.clear();
	mModelMaterial.clear();

	// Update the rolling cube's position
	for(unsigned int i=0; i< mMotionCubes.size(); i ++ )
	{
		// If render time is more than 1000 frames, than ignore the cube.
		if(mMotionCubes[i].ElapsedTime > 1000)
			continue;

		// Increase elapsed time
		mMotionCubes[i].ElapsedTime += 0.5f;

		// Translate the model
		glm::mat4 model(1.0);
		glm::vec3 translateDistance = mMotionCubes[i].StartPos + mMotionCubes[i].Speed * mMotionCubes[i].ElapsedTime * mMotionCubes[i].MoveDirection;
		model = glm::translate(model,translateDistance);

		// Scale the model
		glm::vec3 scale(0.2,0.2,0.2);
		model = glm::scale(model,scale);

		// Rotate the model by Z Axis
		float rotateAngle = mMotionCubes[i].ElapsedTime * 1.0f;
		model = glm::rotate(model,rotateAngle,glm::vec3(0.0,0.0,1.0));

		mModelCoordinateData.push_back(model);
		mModelMaterial.push_back(mMotionCubes[i].Material);
	}

	mCubeProg.InstanceCount = mModelCoordinateData.size();

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

	mCubeProg.VertexCount = mVertexData.size();
}

void MagicCube::initializeCamera()
{
	glm::vec3 cameraPos = glm::vec3(CUBEMARGIN * CUBEXROWS / 2,CUBEMARGIN * CUBEYROWS / 2, 30);

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

}

void MagicCube::onMouseDown(SDL_MouseButtonEvent button)
{
	if(button.button == SDL_BUTTON_LEFT && button.state== SDL_PRESSED)
	{
		// Launch a cube
		int materialID = mMotionCubes.size()%3 + 1;
		float speed = LaunchSpeed * materialID * materialID;
		MotionCube cube(mCamera->getPos(),mCamera->getLookatDirection(),speed,0.0f,(CubeMaterial)materialID);
		mMotionCubes.push_back(cube);

		SDL_Log("Material id = %d",materialID);
		SDL_Log("Launch cube, current count is %d",mMotionCubes.size());
	}
}