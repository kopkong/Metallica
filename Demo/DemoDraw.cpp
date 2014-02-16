#include "DemoDraw.h"


DemoDraw::DemoDraw(int,int)
{
	mDataMgr = new DataHelper();
	mSceneCamera = new Camera(mWindowWidth,mWindowHeight);
	mTerrainRender = new TerrainRender(mSceneCamera);
}

DemoDraw::~DemoDraw(void)
{
	delete mDataMgr;
}

bool DemoDraw::initData()
{
	return mDataMgr->initAll();
}

bool DemoDraw::initEngine()
{
	mTerrainRender->init();

	return true;
}

void DemoDraw::updateScene()
{

}

void DemoDraw::drawScene()
{
	mTerrainRender->render();
	SDL_GL_SwapWindow(mSDLMainWindow);
}

void DemoDraw::drawBackGround()
{

}


