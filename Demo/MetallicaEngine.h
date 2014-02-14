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
	//bool CreateGameWindow();
	void updateScene();
	void drawScene();
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

	SDL_Window *mMainWindow;
	SDL_GLContext mMainContext;
};

