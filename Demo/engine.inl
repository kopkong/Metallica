#pragma once

#include <stdlib.h>
#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "../external/gel/stxutif.h"

using namespace std;

static int seed = 0;

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