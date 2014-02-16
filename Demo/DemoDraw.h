#pragma once
#include "metallicaengine.h"
#include "DataHelper.h"
#include "TerrainRender.h"
#include "camera.h"

class DemoDraw :
	public MetallicaEngine
{
public:
	DemoDraw(int,int);
	~DemoDraw(void);
	bool initData();
	bool initEngine();
	void updateScene();
	void drawScene();
	void drawBackGround();
	DataHelper *mDataMgr;

	int mWindowWidth;
	int mWindowHeight;
	

private:
	Camera* mSceneCamera;
	TerrainRender* mTerrainRender;
};

