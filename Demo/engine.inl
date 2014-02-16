#pragma once

#include "stdafx.h"
#include "IL/ilu.h"

using namespace std;

static int seed = 0;

struct vertex_v2fv2f
{
	glm::vec2 Position;
	glm::vec2 Texcoord;
};

struct vertex_v3fv2f
{
	vertex_v3fv2f
	(
		glm::vec3 const & Position,
		glm::vec2 const & Texcoord
	) :
		Position(Position),
		Texcoord(Texcoord)
	{}

	glm::vec3 Position;
	glm::vec2 Texcoord;
};

inline int convertStrToInt(string str)
{
	return atoi(str.c_str());
}

inline int convertWStrToInt(wstring str)
{
	const wchar_t* wc = str.c_str();
	size_t len = wcslen(wc) + 1;
	size_t converted = 0;

	char* ch = (char*)malloc(len*sizeof(char));
	wcstombs_s(&converted, ch, len, wc, _TRUNCATE);
	return atoi(ch);
}

inline float random()
{
	srand(seed);
	seed = rand();
	return rand() / (float)RAND_MAX;
}

inline int randomRange(int low,int high)
{
	int range = high - low;

	if( range > 0)
	{
		return (int) (random() * range) + low;
	}
	else
	{
		return low;
	}
}

inline ifstream loadANSIFile(string fileName)
{
	ifstream fin;
	string path1 = "data/" + fileName;
	string path2 = "../data/" + fileName;

	// search first path
	fin.open(path1);
	if(fin)
	{
		return fin;
	}
	else
	{
		fin.clear();
		fin.close();
	}

	// search second path
	fin.open(path2);
	if(fin)
	{
		return fin;
	}
	else
	{
		fin.clear();
		fin.close();
	}
	
	return fin;

}

inline wifstream loadUTF8File(string fileName)
{
	wifstream wfin;
	//wfin.imbue(gel::stdx::utf8_locale);

	string path1 = "data/" + fileName;
	string path2 = "../data/" + fileName;

	// search first path
	wfin.open(path1);
	if(wfin)
	{
		return wfin;
	}
	else
	{
		wfin.clear();
		wfin.close();
	}

	// search second path
	//wfin.imbue(gel::stdx::utf8_locale);
	wfin.open(path2);
	if(wfin)
	{
		return wfin;
	}
	else
	{
		wfin.clear();
		wfin.close();
	}
	
	return wfin;

}

inline void SDLDie(const char *msg)
{
	printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}

inline void splitString(string str,char delim,string* strArray,int arrayLength)
{
	//string newStr(str,str.size()-1);
	string newStr(str);

	int i = 0;
	int startPos = 0;
	string tmpStr;

	int delimPos = newStr.find(delim,startPos);

	while( delimPos != string::npos && i< arrayLength)
	{
		tmpStr = newStr.substr(startPos,delimPos - startPos);
		strArray[i] = tmpStr;
		SDL_Log(tmpStr.c_str());

		startPos = delimPos + 1;
		delimPos = newStr.find(delim,startPos);
		i++;

	}
}

inline void splitWString(wstring str,char delim,wstring* strArray,int arrayLength)
{
	wstring newStr(str);

	int i = 0;
	int startPos = 0;
	wstring tmpStr;

	int delimPos = newStr.find(delim,startPos);

	while( delimPos != string::npos && i< arrayLength)
	{
		tmpStr = newStr.substr(startPos,delimPos - startPos);
		strArray[i] = tmpStr;
		//SDL_Log(tmpStr.c_str());

		startPos = delimPos + 1;
		delimPos = newStr.find(delim,startPos);
		i++;

	}
}

// GL help functions
inline bool checkError(const char* Title)
{
	int Error;
	if((Error = glGetError()) != GL_NO_ERROR)
	{
		std::string ErrorString;
		switch(Error)
		{
		case GL_INVALID_ENUM:
			ErrorString = "GL_INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			ErrorString = "GL_INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			ErrorString = "GL_INVALID_OPERATION";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			ErrorString = "GL_INVALID_FRAMEBUFFER_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			ErrorString = "GL_OUT_OF_MEMORY";
			break;
		default:
			ErrorString = "UNKNOWN";
			break;
		}
		fprintf(stdout, "OpenGL Error(%s): %s\n", ErrorString.c_str(), Title);

		//pFile = fopen("log.txt","w");
		//fprintf(pFile, "OpenGL Error(%s): %s\n", ErrorString.c_str(), Title);
		//fclose(pFile);
	}
	return Error == GL_NO_ERROR;
}

inline void compileShaderFromFile(const char* shader_file_path, GLuint shaderType,GLuint programID)
{
	GLint Result = GL_FALSE;
	int InfoLogLength;
	GLuint ShaderID;

	switch(shaderType)
	{
		case GL_VERTEX_SHADER:
			{
				ShaderID = glCreateShader(GL_VERTEX_SHADER);
				break;
			}
		case GL_GEOMETRY_SHADER:
			{
				ShaderID = glCreateShader(GL_GEOMETRY_SHADER);
				break;
			}
		case GL_FRAGMENT_SHADER:
			{
				ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
				break;
			}
	}

	// Read the Shader code from the file
	std::string ShaderCode;
	std::ifstream ShaderStream(shader_file_path, std::ios::in);
	if(ShaderStream.is_open()){
		std::string Line = "";
		while(getline(ShaderStream, Line))
			ShaderCode += "\n" + Line;
		ShaderStream.close();
	}

	//  Compile Shader
	printf("Compiling shader : %s\n", shader_file_path);
	char const * SourcePointer = ShaderCode.c_str();
	glShaderSource(ShaderID, 1, &SourcePointer , NULL);
	glCompileShader(ShaderID);

	// Check  Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ShaderErrorMessage(InfoLogLength);
	glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, &ShaderErrorMessage[0]);
	fprintf(stdout, "%s\n", &ShaderErrorMessage[0]);

	// Link the program
	fprintf(stdout, "Linking program\n");
	glAttachShader(programID, ShaderID);
	glLinkProgram(programID);

	// Check the program
	glGetProgramiv(programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
	glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

	glDeleteShader(ShaderID);

}

inline bool loadTexture2D(const wchar_t* file_name,GLuint textureID)
{
	ILuint m_imageID = 0;
	ILboolean success(0);
	//GLuint textureID;

	ilInit();
	ilEnable(IL_ORIGIN_SET);
	ilGenImages(1,&m_imageID);
	ilBindImage(m_imageID);

	success = ilLoadImage(file_name);

	if(!success)
	{
		ILenum errorCode = ilGetError();
		wprintf(iluErrorString(errorCode));
		//load fail
		ilDeleteImages(1,&m_imageID);
		return false;
	}

	printf("Load texture success \n");

	ilConvertImage(IL_RGBA,IL_UNSIGNED_BYTE);

	glBindTexture(GL_TEXTURE_2D,textureID);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,ilGetInteger(IL_IMAGE_WIDTH),ilGetInteger(IL_IMAGE_HEIGHT),0,GL_RGBA,GL_UNSIGNED_BYTE,ilGetData());

	return true;
}