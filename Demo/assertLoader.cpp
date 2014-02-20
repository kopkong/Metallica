#include "StdAfx.h"
#include "assertLoader.h"
#include "IL\il.h"
#include <fstream>
#include "engine.inl"

assertLoader::assertLoader(void)
{
}


assertLoader::~assertLoader(void)
{
}

bool assertLoader::import(const string dir,const string file)
{
	// set dir first
	mBaseDir = dir;

	//check if file exists
	string fileFullName = dir + file;
	std::ifstream fin(fileFullName.c_str());
	if(!fin.fail())
	{
		fin.close();
	}
	else
	{
		SDL_Log("Couldn't open file: %s",file.c_str());
		return false;
	}

	mScene = mImporter.ReadFile( fileFullName, aiProcessPreset_TargetRealtime_Quality);

	// If the import failed, report it
	if( !mScene)
	{
		SDL_Log( mImporter.GetErrorString());
		return false;
	}

	// Now we can access the file's contents.
	SDL_Log("Import of scene %s succeeded.",file.c_str());

	// We're done. Everything will be cleaned up by the importer destructor
	return true;

}

bool assertLoader::loadTextures(aiScene* scene)
{
	ILboolean success;

	/* Before calling ilInit() version should be checked. */
	if (ilGetInteger(IL_VERSION_NUM) < IL_VERSION)
	{
		ILint test = ilGetInteger(IL_VERSION_NUM);
		/// wrong DevIL version ///
		std::string err_msg = "Wrong DevIL version. Old devil.dll in system32/SysWow64?";
		char* cErr_msg = (char *) err_msg.c_str();
		SDLDie(cErr_msg);
		
		return false;
	}

	ilInit(); /* Initialization of DevIL */

	if (scene->HasTextures()) SDLDie("Support for meshes with embedded textures is not implemented");

	/* getTexture Filenames and Numb of Textures */
	for (unsigned int m=0; m<scene->mNumMaterials; m++)
	{
		int texIndex = 0;
		aiReturn texFound = AI_SUCCESS;

		aiString path;	// filename

		while (texFound == AI_SUCCESS)
		{
			texFound = scene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, texIndex, &path);
			mTextureIdMap[path.data] = NULL; //fill map with textures, pointers still NULL yet
			texIndex++;
		}
	}

	int numTextures = mTextureIdMap.size();

	/* array with DevIL image IDs */
	ILuint* imageIds = NULL;
	imageIds = new ILuint[numTextures];

	/* generate DevIL Image IDs */
	ilGenImages(numTextures, imageIds); /* Generation of numTextures image names */

	/* create and fill array with GL texture ids */
	mTextureIds = new GLuint[numTextures];
	glGenTextures(numTextures, mTextureIds); /* Texture name generation */

	/* define texture path */
	//std::string texturepath = "../../../test/models/Obj/";

	/* get iterator */
	std::map<string, GLuint*>::iterator itr = mTextureIdMap.begin();

	for (int i=0; i<numTextures; i++)
	{
		//save IL image ID
		string filename = (*itr).first;  // get filename
		(*itr).second =  &mTextureIds[i];	  // save texture id for filename in map
		itr++;								  // next texture

		ilBindImage(imageIds[i]); /* Binding of DevIL image name */
		string fileloc = mBaseDir + filename;	/* Loading of image */
		wchar_t* wFileloc;
		getWC(fileloc.c_str(),wFileloc);
		success = ilLoadImage(wFileloc);

		if (success) /* If no error occured: */
		{
			success = ilConvertImage(IL_RGB, IL_UNSIGNED_BYTE); /* Convert every colour component into
			unsigned byte. If your image contains alpha channel you can replace IL_RGB with IL_RGBA */
			if (!success)
			{
				/* Error occured */
				SDL_Log("Couldn't convert image");
				return false;
			}
			//glGenTextures(numTextures, &textureIds[i]); /* Texture name generation */
			glBindTexture(GL_TEXTURE_2D, mTextureIds[i]); /* Binding of texture name */
			//redefine standard texture values
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 

			glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), ilGetInteger(IL_IMAGE_WIDTH),
				ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE,
				ilGetData()); /* Texture specification */
		}
		else
		{
			/* Error occured */
			SDL_Log("Couldn't load Image: %s" , fileloc.c_str() );
		}


	}

	ilDeleteImages(numTextures, imageIds); /* Because we have already copied image data into texture data
	we can release memory used by image. */

	//Cleanup
	delete [] imageIds;
	imageIds = NULL;

	//return success;
	return true;
}
