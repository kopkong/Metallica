#include "DemoDraw.h"

DemoDraw::DemoDraw(void)
{
	mDataMgr = new DataHelper();
}


DemoDraw::~DemoDraw(void)
{
	delete mDataMgr;
}

bool DemoDraw::InitData()
{
	return mDataMgr->initAll();
}
