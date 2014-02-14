#pragma once
#include "metallicaengine.h"
#include "DataHelper.h"

class DemoDraw :
	public MetallicaEngine
{
public:
	DemoDraw(void);
	DemoDraw(int,int);
	~DemoDraw(void);
	bool initData();
	void updateScene();
	void drawScene();
	void drawBackGround();
	void initProg();
	void initTexture();
	DataHelper *mDataMgr;

	int mWindowWidth;
	int mWindowHeight;
};

