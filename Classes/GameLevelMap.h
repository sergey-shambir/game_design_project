#pragma once

#include "cocos2dgame.h"

class GameLevelMap : public cocos2d::TMXTiledMap
{
public:
	static GameLevelMap *create(const std::string &tmxFile);

	cocos2d::Size getMapVisibleSize() const;

private:
	void initWithTMXFile(const std::string &tmxFile);
	void loadUnits();
	cocos2d::TMXObjectGroup *getObjectGroupOrThrow(const std::string &name);
	cocos2d::Rect getObjectRect(const cocos2d::ValueMap &properties) const;
	bool getOptionalBool(const cocos2d::ValueMap &properties, const std::string &name) const;
	cocos2d::RefPtr<cocos2d::Sprite> spawnObjectSprite(const std::string &frameName, const cocos2d::ValueMap &properties) const;
};
