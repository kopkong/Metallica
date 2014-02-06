#include "BattleUnit.h"
#include "SDL_timer.h"
#include "locale.h"

BattleUnit::BattleUnit(void)
{
	//initialize member
	mMaxHP = 100;
	mCurHP = 100;
	mTongLv = 0;
	mWuLi = 0;
	mZhiLi = 0;
	mZhengZhi = 0;
	mMeili = 0;
}


BattleUnit::~BattleUnit(void)
{
}

//BattleUnit::BattleUnit(const BattleUnit &battleUnit)
//{
//	//mLogName.c_str() = battleUnit.mLogName.c_str();
//}

// Return false if you have died
bool BattleUnit::attack(BattleUnit *enemy)
{
	SDL_Log("=================%s start to attack %s =========== ",mLogName.c_str(),enemy->mLogName.c_str());

	// already dead
	if(isDead())
	{
		SDL_Log("%s Current HP is below than 0, you are dead! ",mLogName.c_str());
		return false;
	}
	
	// Own attack
	int rd1 = randomRange(0,ATTACK_PROPERTIES);
	int attackPoints1 = seekforAttackPoint(rd1);
	//SDL_Log("%s Get first random number is : %d",mLogName.c_str(),rd1);
	SDL_Log("%s attack point is : %d", mLogName.c_str(), attackPoints1);

	// Emeny's attack
	int rd2 = randomRange(0,ATTACK_PROPERTIES);
	int attackPoints2 = enemy->seekforAttackPoint(rd2);
	//SDL_Log("%s Get second random number is : %d",enemy->mLogName.c_str(), rd2);
	SDL_Log("%s attack point is : %d" , enemy->mLogName.c_str(), attackPoints2);

	// Minus
	int duelResult = attackPoints1 - attackPoints2 ;
	bool firstWinnder = (duelResult >=0);
	unsigned int realDamage = abs(duelResult);

	if(!firstWinnder)
	{
		if(mCurHP < realDamage)
		{
			mCurHP = 0;
		}
		else
		{
			mCurHP -= realDamage;
		}
		SDL_Log("%s lost %d hp, current hp is %d",mLogName.c_str(), realDamage, mCurHP);
	}
	else
	{
		if(enemy->mCurHP < realDamage )
		{
			enemy->mCurHP = 0;
		}
		else
		{
			enemy->mCurHP -= realDamage;
		}
		SDL_Log("%s lost %d hp, current hp is %d", enemy->mLogName.c_str(), realDamage, enemy->mCurHP);
	}


	if(isDead())
	{
		SDL_Log("%s is dead after duel !",mLogName.c_str());
		return false;
	}
	else
	{
		return true;
	}
}

bool BattleUnit::isDead()
{
	return mCurHP <= 0;
}

unsigned int BattleUnit::seekforAttackPoint(unsigned int pIndex)
{
	switch(pIndex)
	{
		case ATTACK_PROPERTYINDEX_TONGLV:
			return mTongLv;
		case ATTACK_PROPERTYINDEX_WULI:
			return mWuLi;
		case ATTACK_PROPERTYINDEX_ZHILI:
			return mZhiLi;
		case ATTACK_PROPERTYINDEX_ZHENGZHI:
			return mZhengZhi;
		case ATTACK_PROPERTYINDEX_MEILI:
			return mMeili;
		default:
			return 0;
	}

	return 0;
}

void BattleUnit::heal(unsigned int healPoint)
{
	if(mCurHP == mMaxHP)
	{
		SDL_Log("Full HP, no need to heal!");
		return;
	}

	unsigned int tmpHP = mCurHP + healPoint;

	if(tmpHP >= mMaxHP)
	{
		mCurHP = mMaxHP;
	}
	else
	{
		mCurHP = tmpHP;
	}

	SDL_Log("Success healing, curHP is %d",mCurHP);
}
