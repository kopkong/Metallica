#pragma once
#include <stdio.h>

#include "SDL.h"
#include "GL\glew.h"

class MetallicaEngine
{
public:
	// public function
	MetallicaEngine(void);
	~MetallicaEngine(void);
	bool Init();
	//bool CreateGameWindow();
	void UpdateScene();
	void DrawScene();
	void Run();
	void DestoryEngine();
	void SDLDie(const char *msg);
	void UserEventHandler(SDL_Event*);
	void KeyBoardHandler();
	void MouserHandler();

	// public member
	bool mEngineStopped;

	const char* mWindowTitleName;
	int mWindowWidth;
	int mWindowHeight;

	SDL_Window *mMainWindow;
	SDL_GLContext mMainContext;
};

