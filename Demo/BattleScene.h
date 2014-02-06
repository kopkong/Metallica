#pragma once
#include "StdAfx.h"
#include "HeroCard.h"

using namespace std;

class BattleScene
{
public:
	BattleScene(void);
	~BattleScene(void);

	wstring mBattleSceneName;
	wstring mSideOneName;
	wstring mSideTwoName;
	char mSideOneMembers;
	char mSideTwoMembers;
	vector<HeroCard*> mSideOneHeroes;
	vector<HeroCard*> mSideTwoHeroes;
	bool mBattleStarted;
	bool mBattlePaused;
	bool mBattleOver;
	int mBattleRound;
	
	// attackIndex beat attackedIndex
	void battleAttack(int sider,int attackIndex, int beAttackedIndex);
	void resetBattle();
	bool createBattleSide(int sideOneMembers,int sideTwoMembers,wstring,wstring,vector<HeroCard*> sideOneHeroes,vector <HeroCard*> sideTwoHeroes);
	void defeated(int sider);
	void destoryBattleScene();
	void autoAttack();

private:
	string mLogSideOneName;
	string mLogSideTwoName;
	string mLogBattleSceneName;
};

