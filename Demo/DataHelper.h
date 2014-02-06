#pragma once
#include "StdAfx.h"

#include "HeroCard.h"
#include "City.h"
#include "Role.h"
#include <vector>

class DataHelper
{
public:
	DataHelper(void);
	~DataHelper(void);
	vector<HeroCard> mHeroCards;
	bool initAll();

private:
	int mCityCount;
	City mCities[MAX_CITIES];
	char mCityAdjcentMatrix[MAX_CITIES][MAX_CITIES];
	Role *mRoleInfo;

	bool loadHeroCards();
	bool loadCities();
	bool loadRoleInfo();

	bool saveRoleInfo();
};

