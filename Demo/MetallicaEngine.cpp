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
	DestoryEngine();
}

bool MetallicaEngine::Init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		SDLDie("Unable to initialize SDL");
		return false;
	}

	// Initialize GLEW
	//glewExperimental = true;
	//if(glewInit() != GLEW_OK)
	//{
	//	SDLDie("Failed to initialize GLEW\n");
	//	return false;
	//}

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

void MetallicaEngine::UpdateScene()
{

}

void MetallicaEngine::DrawScene()
{
	glClearColor(1.0,0.0,0.0,1.0);
	SDL_GL_SwapWindow(mMainWindow);
}

void MetallicaEngine::Run()
{
	SDL_Event event;

	while((!mEngineStopped) && (SDL_WaitEvent(&event)))
	{
		UpdateScene();
		DrawScene();

		switch(event.type)
		{
			case SDL_USEREVENT:
				UserEventHandler(&event);
				break;
			case SDL_KEYDOWN:
				break;
			case SDL_MOUSEBUTTONDOWN:
				break;
			case SDL_QUIT:
				mEngineStopped = true;
				break;
			default:
				break;
		}
	}
}

void MetallicaEngine::DestoryEngine()
{
	// Delete OpenGL context, destory window
	SDL_GL_DeleteContext(mMainContext);
	SDL_DestroyWindow(mMainWindow);
	SDL_Quit();
}

void MetallicaEngine::SDLDie(const char *msg)
{
    printf("%s: %s\n", msg, SDL_GetError());
    SDL_Quit();
    exit(1);
}

void MetallicaEngine::UserEventHandler(SDL_Event* event)
{
	// Handle event in child class

}


