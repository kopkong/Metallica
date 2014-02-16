#include "MetallicaEngine.h"

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

	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,2);

	//SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	//SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,24);

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
	if(!initEngine())
	{
		SDLDie("Unable to init engine!");
		return false;
	}

	SDL_GL_SetSwapInterval(1);
	checkError("Init Engine");

	return true;
}

void MetallicaEngine::run()
{
	SDL_Event event;

	while((!mEngineStopped) && (SDL_WaitEvent(&event)))
	{
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
				updateScene();
				drawScene();
				break;
		}
	}
}

void MetallicaEngine::destoryEngine()
{
	// Delete OpenGL context, destory window
	SDL_GL_DeleteContext(mSDLMainContext);
	SDL_DestroyWindow(mSDLMainWindow);
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




