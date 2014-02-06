#pragma once
#include "StdAfx.h"
#include "SDL.h"
#include "engine.inl"

using namespace std;

class BattleUnit
{
public:
	BattleUnit(void);
	~BattleUnit(void);
	//BattleUnit(const BattleUnit &battleUnit);
	//BattleUnit(int hp, int tl, int wl, int zl, int zz, int ml); 

	// property
	unsigned int mMaxHP;
	unsigned int mCurHP;
	unsigned int mTongLv;
	unsigned int mWuLi;
	unsigned int mZhiLi;
	unsigned int mZhengZhi;
	unsigned int mMeili;

	// identity
	string mLogName;

	bool attack(BattleUnit *unit);
	bool isDead();
	unsigned int seekforAttackPoint(unsigned int propertyIndex);
	void heal(unsigned int healPoints);

};

