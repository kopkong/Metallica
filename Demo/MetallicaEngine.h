#pragma once

#include "stdafx.h"

class MetallicaEngine
{
public:
	// public function
	MetallicaEngine(void);
	~MetallicaEngine(void);
	bool init();
	virtual bool initData() = 0;
	virtual bool initEngine() = 0;
	//bool CreateGameWindow();
	virtual void updateScene() = 0;
	virtual void drawScene() = 0;
	void run();
	void destoryEngine();
	//void SDLDie(const char *msg);
	void userEventHandler(SDL_Event*);
	void keyBoardHandler(SDL_Event*);
	void mouseHandler(SDL_Event*);

	// public member
	bool mEngineStopped;

	const char* mWindowTitleName;
	int mWindowWidth;
	int mWindowHeight;

	SDL_Window *mSDLMainWindow;
	SDL_GLContext mSDLMainContext;
};

