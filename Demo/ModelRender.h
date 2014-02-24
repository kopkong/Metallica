#pragma once
#include "StdAfx.h"
#include "camera.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <map>
using namespace std;

class ModelRender
{
public:
	ModelRender(Camera*,const string ,const string);
	~ModelRender(void);

	void init();
	void render();
	void exit();
	Camera *mCamera;
private:
	string mModelDir;
	string mModelFileName;
	Assimp::Importer mImporter;
	aiVector3D mSceneMin,mSceneMax,mSceneCenter;
	const aiScene* mScene;

	GLuint mProgName;
	GLuint mUniformTexture;
	GLuint mUniformMVP;
	map<string,GLuint*> mTexturesMap;
	GLuint* mTextureIDs;

	bool import();
	void initializeCamera();
	bool initializeTexture();
	void initializeMesh();
	
};

