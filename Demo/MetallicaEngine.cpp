#include "MetallicaEngine.h"
#include "engine.inl"

MetallicaEngine::MetallicaEngine(void)
{
	mWindowTitleName = "Test";
	mWindowWidth = 512;
	mWindowHeight = 512;

	mEngineStopped = false;
}


MetallicaEngine::~MetallicaEngine(void)
{
	destoryEngine();
}

bool MetallicaEngine::init()
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

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

	mMainWindow = SDL_CreateWindow(mWindowTitleName,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,
		mWindowWidth,mWindowHeight,SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	if(!mMainWindow)
	{
		SDLDie("Unable to create Window!");
		return false;
	}

	mMainContext = SDL_GL_CreateContext(mMainWindow);

	SDL_GL_SetSwapInterval(1);

	glClear(GL_COLOR_BUFFER_BIT);

	return true;
}

void MetallicaEngine::updateScene()
{

}

void MetallicaEngine::drawScene()
{
	glClearColor(1.0,0.0,0.0,1.0);
	SDL_GL_SwapWindow(mMainWindow);
}

void MetallicaEngine::run()
{
	SDL_Event event;

	while((!mEngineStopped) && (SDL_WaitEvent(&event)))
	{
		updateScene();
		drawScene();

		switch(event.type)
		{
			case SDL_USEREVENT:
				userEventHandler(&event);
				break;
			case SDL_KEYDOWN:
				keyBoardHandler(&event);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouseHandler(&event);
				break;
			case SDL_QUIT:
				mEngineStopped = true;
				break;
			default:
				break;
		}
	}
}

void MetallicaEngine::destoryEngine()
{
	// Delete OpenGL context, destory window
	SDL_GL_DeleteContext(mMainContext);
	SDL_DestroyWindow(mMainWindow);
	SDL_Quit();
}

void MetallicaEngine::userEventHandler(SDL_Event* event)
{
	// Handle event in child class

}

void MetallicaEngine::keyBoardHandler(SDL_Event* event)
{
	SDL_Log("Press key %d",event->key.keysym.scancode);
}

void MetallicaEngine::mouseHandler(SDL_Event* event)
{
	SDL_Log("Mouse click %d",event->button.button);
}




