#include "AbstractScene.h"

bool AbstractScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	try
	{
		tryInit();
		return true;
	}
	catch (const std::exception& ex)
	{
		CCLOGERROR("cannot load scene due exception: %s", ex.what());
		return false;
	}
}
