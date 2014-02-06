#pragma once
#include "metallicaengine.h"
#include "DataHelper.h"

class DemoDraw :
	public MetallicaEngine
{
public:
	DemoDraw(void);
	~DemoDraw(void);
	bool InitData();
	DataHelper *mDataMgr;
};

