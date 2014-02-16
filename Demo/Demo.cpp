// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DemoDraw.h"
#include "BattleUnit.h"
#include "HeroCard.h"
#include "BattleScene.h"
#include <iostream>

DemoDraw demo(WINDOW_WIDTH,WINDOW_HEIGHT);

void testBattle()
{
	BattleScene scene;
	
	HeroCard *c1 = new HeroCard(demo.mDataMgr->mHeroCards[0]);
	HeroCard *c2 = new HeroCard(demo.mDataMgr->mHeroCards[1]);
	HeroCard *c3 = new HeroCard(demo.mDataMgr->mHeroCards[2]);

	vector<HeroCard *> vector1;
	vector<HeroCard *> vector2;

	vector1.push_back(c1);
	vector1.push_back(c2);
	vector2.push_back(c3);

	scene.createBattleSide(2,1,L"A",L"B",vector1,vector2);

	scene.autoAttack();
}

void test()
{
	wstring a = L"aaa";
	string aa(a.begin(),a.end());
	string ab("bbb");
	char* c = "cccc";
	SDL_Log("test wstring : %s ",aa.c_str());
	SDL_Log("test string : %s ",ab.c_str());

	std::cout <<"std::string =     "<<ab<<std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(!demo.init())
	{
		SDL_Log("Engine Init failed!");
	}

	//test();

	//testBattle();

	demo.run();
	
	return 0;
}



