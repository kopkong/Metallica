#pragma once
#include "battleunit.h"

class HeroCard :	public BattleUnit
{
public:
	HeroCard(void);
	~HeroCard(void);

	// public member
	wstring mName;
	wstring mCourtesyName;
	int mGender;
	int mBornYear;
	string imgID;
};

