#include "BattleScene.h"


BattleScene::BattleScene(void)
{
	mLogBattleSceneName = "Default Battle Scene";
	mLogSideOneName = "Side One";
	mLogSideTwoName = "Side Two";

	mSideOneMembers = 0;
	mSideTwoMembers = 0;
	mBattleStarted = false;
	mBattlePaused = false;
	mBattleOver =	false;
	mBattleRound = 0;
}


BattleScene::~BattleScene(void)
{
}

bool BattleScene::createBattleSide(int membersOne,int membersTwo,wstring nameOne,wstring nameTwo,
	vector<HeroCard*> sideOneHeroes,vector <HeroCard*> sideTwoHeroes)
{
	if(membersOne > BATTLE_MAX_HEROES || membersTwo > BATTLE_MAX_HEROES)
	{
		SDL_Log("Side members above MAX count!");
		return false;
	}

	mSideOneMembers = membersOne;
	mSideTwoMembers = membersTwo;
	mSideOneName = nameOne;
	mSideTwoName = nameTwo;

	if(sideOneHeroes.size() >= membersOne)
	{
		for(vector<HeroCard *>::size_type ix = 0; ix < membersOne; ++ ix)
		{
			mSideOneHeroes.push_back(sideOneHeroes[ix]);
		}
	}
	else
	{
		SDL_Log("Side one don't have enough heroes!");
		return false;
	}


	if(sideTwoHeroes.size() >= membersTwo)
	{
		for(vector<HeroCard *>::size_type ix =0; ix <membersTwo; ++ ix)
		{
			mSideTwoHeroes.push_back(sideTwoHeroes[ix]);
		}
	}
	else
	{
		SDL_Log("Side two don't have enough heroes!");
		return false;
	}

	return true;
}

void BattleScene::battleAttack(int sider,int idx1, int idx2)
{
	if(sider != BATTLE_SIDE_ONE || sider != BATTLE_SIDE_TWO)
	{
		SDL_Log("Unknown sider");
		return;
	}

	if(sider == BATTLE_SIDE_ONE)
	{
		if(mSideOneHeroes[idx1]->isDead())
		{
			SDL_Log("Hero %s is dead, can't attack others",mSideOneHeroes[idx1]->mLogName);
		}
		else
		{
			mSideOneHeroes[idx1]->attack(mSideTwoHeroes[idx2]);
		}
		mBattleRound ++;

		SDL_Log("***************Round %d ******************",mBattleRound);
	}

	if(sider == BATTLE_SIDE_TWO)
	{
		if(mSideTwoHeroes[idx1]->isDead())
		{
			SDL_Log("Hero %s is dead, can't attack others",mSideTwoHeroes[idx1]->mLogName);
		}
		else
		{
			mSideTwoHeroes[idx1]->attack(mSideOneHeroes[idx2]);
		}
		mBattleRound ++;

		SDL_Log("***************Round %d ******************",mBattleRound);
	}
}

void BattleScene::autoAttack()
{
	// side A attacks side B
	// until all memers of A or B are dead

	int team1Member = 0;
	int team2Member = 0;

	while(!mBattleOver)
	{
		if(mSideOneHeroes[team1Member]->isDead())
		{
			team1Member ++;
		}

		if(mSideTwoHeroes[team2Member]->isDead())
		{
			team2Member ++;
		}

		// check is over or not
		if(team1Member >= mSideOneMembers)
		{
			defeated(BATTLE_SIDE_ONE);
			mBattleOver = true;
			continue;
		}

		if(team2Member >= mSideTwoMembers)
		{
			defeated(BATTLE_SIDE_TWO);
			mBattleOver = true;
			continue;
		}

		// both team are not defeated
		mSideOneHeroes[team1Member]->attack(mSideTwoHeroes[team2Member]);

	}

}

void BattleScene::defeated(int sider)
{
	if(sider == BATTLE_SIDE_ONE)
	{
		SDL_Log("%s is defeated",mLogSideOneName.c_str());
	}
	else
	{
		SDL_Log("%s is defeated", mLogSideTwoName.c_str());
	}

	mBattleOver = true;
}
	


