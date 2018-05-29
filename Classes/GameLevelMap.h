#pragma once

#include "cocos2dgame.h"

class GameLevelMap : public cocos2d::TMXTiledMap
{
public:
	static GameLevelMap* create(const std::string& tmxFile);

	cocos2d::Size getMapVisibleSize() const;

private:
	void initWithTMXFile(const std::string& tmxFile);
	void loadUnits();
	cocos2d::TMXObjectGroup* getObjectGroupOrThrow(const std::string& name);
};
