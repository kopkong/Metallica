// Demo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DemoDraw.h"

int _tmain(int argc, _TCHAR* argv[])
{
	DemoDraw demo;

	if(!demo.Init())
	{
		printf("Engine Init failed!\n");
	}

	demo.Run();
	
	return 0;
}

