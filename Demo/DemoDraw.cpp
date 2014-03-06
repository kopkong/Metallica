#include "DemoDraw.h"


DemoDraw::DemoDraw(int width,int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mDataMgr = new DataHelper();
	mSceneCamera = new Camera(mWindowWidth,mWindowHeight);
	mTerrainRender = new TerrainRender(mSceneCamera);
	mMagicCube = new MagicCube(mSceneCamera);
	mModelRender = new ModelRender(mSceneCamera,"../external/assimp--3.0.1270-sdk/test/models/OBJ/", "spider.obj");
}

DemoDraw::~DemoDraw(void)
{
	delete mDataMgr;
}

bool DemoDraw::init()
{
	// Init Data
	if(!initData())
	{
		SDLDie("Unable to initialize Game data!");
		return false;
	}
	else
	{
		SDL_Log("Init Game data success!");
	}

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDLDie("Unable to initialize SDL");
		return false;
	}
	else
	{
		SDL_Log("Init SDL success!");
	}

	mSDLMainWindow = SDL_CreateWindow(mWindowTitleName,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		mWindowWidth,mWindowHeight,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(!mSDLMainWindow)
	{
		SDLDie("Unable to create Window!");
		return false;
	}

	mSDLMainContext = SDL_GL_CreateContext(mSDLMainWindow);
	//checkError("Create GL context");

	// init glew
	if(GLEW_OK != glewInit())
	{
		SDLDie("Glew init failed!");
		return false;
	}

	checkError("Init GLEW");

	// init engine
	if(!initRender())
	{
		SDLDie("Unable to init engine!");
		return false;
	}

	SDL_GL_SetSwapInterval(1);
	checkError("Init Engine");

	return true;
}

bool DemoDraw::initData()
{
	return mDataMgr->initAll();
}

bool DemoDraw::initRender()
{
	//mTerrainRender->init();
	//mModelRender->init();
	mMagicCube->init();
	return true;
}

void DemoDraw::updateScene()
{
	mMagicCube->update();
}

void DemoDraw::drawScene()
{
	//mTerrainRender->render();
	//mModelRender->render();
	mMagicCube->render();
	SDL_GL_SwapWindow(mSDLMainWindow);
}

void DemoDraw::drawBackGround()
{

}

void DemoDraw::run()
{
	while((!mEngineStopped))
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_USEREVENT:
					userEventHandler(&event);
					break;
				case SDL_KEYDOWN:
					keyBoardHandler(&event);
					break;
				case SDL_MOUSEBUTTONUP:
					mouseButtonUpHandler(event.button);
					break;
				case SDL_MOUSEBUTTONDOWN:
					mouseButtonDownHandler(event.button);
					break;
				case SDL_MOUSEMOTION:
					mouseMotionHandler(event.button,event.motion);
					break;
				case SDL_MOUSEWHEEL:
					mouseWheelHandler(event.wheel);
					break;
				case SDL_QUIT:
					mEngineStopped = true;
					break;
				default:
					break;
			}
		}
		updateScene();
		drawScene();
	}
}

void DemoDraw::destoryEngine()
{
	// Delete OpenGL context, destory window
	SDL_GL_DeleteContext(mSDLMainContext);
	SDL_DestroyWindow(mSDLMainWindow);
	SDL_Quit();
}

void DemoDraw::userEventHandler(SDL_Event* event)
{
	// Handle event in child class
}

void DemoDraw::keyBoardHandler(SDL_Event* event)
{
	//SDL_Log("Press key %d",event->key.keysym.scancode);
	mSceneCamera->onKeyboard(event->key.keysym.sym);
	mMagicCube->onKeyboard(event->key.keysym.sym);
}

void DemoDraw::mouseButtonUpHandler(SDL_MouseButtonEvent button)
{
	mSceneCamera->onMouseUp(button);
}

void DemoDraw::mouseButtonDownHandler(SDL_MouseButtonEvent button)
{
	//mSceneCamera->onMouseDown(button);
	mMagicCube->onMouseDown(button);
}

void DemoDraw::mouseMotionHandler(SDL_MouseButtonEvent button,SDL_MouseMotionEvent motion)
{
	//mSceneCamera->onMouseMotion(button,motion);
}

void DemoDraw::mouseWheelHandler(SDL_MouseWheelEvent wheel)
{
	mSceneCamera->onMouseWheel(wheel);
}


