#pragma once
#include "StdAfx.h"

#include "HeroCard.h"

class DataHelper
{
public:
	vector<HeroCard> mHeroCards;
	bool initAll();

private:
	//int mCityCount;
	//char mCityAdjcentMatrix[MAX_CITIES][MAX_CITIES];

	bool loadHeroCards();
	bool loadRoleInfo();
	bool saveRoleInfo();
};

