#pragma once

#include "stdafx.h"
#include "camera.h"
#include "engine.inl"
#include <map>

#define CUBEXROWS 4
#define CUBEZROWS 4
#define CUBEYROWS 4

enum CubeMaterial
{
	None,
	Plastic = 1,
	Wood = 2,
	Metal = 3
};

struct MotionCube
{
	MotionCube(glm::vec3 pos, glm::vec3 dir, float v, float t,CubeMaterial m)
	{
		StartPos = pos;
		MoveDirection = dir;
		Speed = v;
		ElapsedTime = t;
		Material = m;
	};

	glm::vec3 StartPos;
	glm::vec3 MoveDirection;
	float Speed;
	float ElapsedTime;
	CubeMaterial Material;
};

typedef struct{
	GLuint MVP;
}TestShaderUniforms;

typedef struct{
	GLuint ProgramId;
	TestShaderUniforms UniformLocations;
	GLuint VAO;
	GLuint VertexBuffer;
	GLuint ElementBuffer;
	unsigned int IndexCount;
}TestProgram;

typedef struct{
	GLuint Projection;
	GLuint ModelView;
	GLuint NormalMatrix;
	GLuint LightPosition;
	GLuint AmbientMaterial;
	GLuint DiffuseMaterial;
	GLuint TessLevelInner;
	GLuint TessLevelOuter;
} TessSphereShaderUniforms;

typedef struct{
	GLuint ProgramId;
	TessSphereShaderUniforms UniformLocations;
	GLuint VAO;
	GLuint VertexBuffer;
	GLuint ElementBuffer;
	unsigned int IndexCount;
} TessSphereShaderProgram;

typedef struct{
	GLuint MVP;
	GLuint Texture;
} CubeShaderUniforms;

typedef struct{
	GLuint ProgramId;
	CubeShaderUniforms UniformLocations;
	GLuint VAO;
	GLuint VertexBuffer;
	GLuint TexturePlastic;
	GLuint TextureWood;
	GLuint TextureMetal;
	unsigned int InstanceCount;
	unsigned int VertexCount;
}CubeShaderProgram;

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
	void onMouseDown(SDL_MouseButtonEvent);

private:
	TessSphereShaderProgram mTessSphereProg;
	TestProgram mTestProg;
	CubeShaderProgram mCubeProg;

	GLuint mProgCubeRendering;
	GLuint mProgSkyBox;
	GLuint mUniformMVP;
	GLuint mUniformCubeTexture;
	GLuint mUniformSkyBoxTexture;

	GLuint mUniformCameraPosition;
	GLuint mUniformViewProjectMatrix;

	GLuint mVertexBuffer;
	GLuint mVertexSkyBox;
	GLuint mTextureCubeFace;

	GLuint mCubeMapSkyBox;

	vector<vertex_v3fv2f> mVertexData;
	vector<MotionCube> mMotionCubes;
	vector<glm::mat4> mModelCoordinateData;
	vector<CubeMaterial> mModelMaterial;
	glm::mat4 mSkyBoxCoordinateData;

	int mSelectedInstance;
	bool mIsPicking;
	glm::vec3 mRayOrigin;
	glm::vec3 mRayDirection;
	glm::ivec2 mMousePos;
	glm::ivec2 mScreenSize;

	void generateCube();
	void initializeCamera();
	void screenPosToWorldRay(int mouseX,int mouseY,int screenWidth,int screenHeight,
		glm::mat4 viewMatrix,glm::mat4 projMatrix,glm::vec3& out_origin,glm::vec3& out_direction);
	bool testRayOBBIntersection(glm::vec3 ray_origin, glm::vec3 ray_direction,glm::vec3 aabb_min,
		glm::vec3 aabb_max, glm::mat4 modelMatrix,float& intersectionDistance);

	void renderRollingCube();
	void renderRayTest();
	void renderSphere();
	void renderSkyBox();
};

