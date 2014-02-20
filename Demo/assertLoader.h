#pragma once
#include "stdafx.h"

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

#include <map>
using namespace std;

class assertLoader
{
public:
	assertLoader(void);
	~assertLoader(void);
	bool import(const string dir,const string file);
	bool loadTextures(aiScene*);

private:
	Assimp::Importer mImporter;
	const aiScene* mScene;
	aiVector3D mSceneMin,mSceneMax,mSceneCenter;
	std::map<string, GLuint*> mTextureIdMap;
	GLuint*	mTextureIds;	
	string mBaseDir; // which directory contains model and texture file
};

