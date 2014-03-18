#pragma once
#include "DataHelper.h"
#include "TerrainRender.h"
#include "ModelRender.h"
#include "camera.h"
#include "MagicCube.h"

class DemoDraw 
{
public:
	DemoDraw(int,int);
	~DemoDraw(void);
	bool init();
	bool initData();
	bool initRender();
	void updateScene();
	void drawScene();
	void drawBackGround();
	void run();
	void destoryEngine();
	void userEventHandler(SDL_Event*);
	void keyBoardHandler(SDL_Event*);
	void mouseButtonDownHandler(SDL_MouseButtonEvent);
	void mouseButtonUpHandler(SDL_MouseButtonEvent);
	void mouseMotionHandler(SDL_MouseMotionEvent);
	void mouseWheelHandler(SDL_MouseWheelEvent);

	DataHelper *mDataMgr;
	bool mEngineStopped;
	const char* mWindowTitleName;
	int mWindowWidth;
	int mWindowHeight;

	SDL_Window *mSDLMainWindow;
	SDL_GLContext mSDLMainContext;
	
private:
	Camera* mSceneCamera;
	TerrainRender* mTerrainRender;
	ModelRender* mModelRender;
	MagicCube* mMagicCube;
};

