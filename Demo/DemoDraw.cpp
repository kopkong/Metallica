#include "DemoDraw.h"

DemoDraw::DemoDraw(void)
{
	mDataMgr = new DataHelper();
}

DemoDraw::DemoDraw(int,int)
{

}

DemoDraw::~DemoDraw(void)
{
	delete mDataMgr;
}

bool DemoDraw::initData()
{
	return mDataMgr->initAll();
}

void DemoDraw::updateScene()
{

}

void DemoDraw::drawScene()
{

}

void DemoDraw::drawBackGround()
{

}


