#pragma once

#include "cocos2dgame.h"
#include <random>

class GameLevelMap : public cocos2d::TMXTiledMap
{
public:
	static GameLevelMap *create(const std::string &tmxFile);

	cocos2d::Size getMapVisibleSize() const;

private:
	void initWithTMXFile(const std::string &tmxFile);
	void loadUnits();
	cocos2d::TMXObjectGroup *getObjectGroupOrThrow(const std::string &name);
	cocos2d::RefPtr<cocos2d::Sprite> spawnGoat(const cocos2d::ValueMap &properties);
	cocos2d::RefPtr<cocos2d::Sprite> spawnObjectSprite(const std::string &frameName, const cocos2d::ValueMap &properties) const;

	cocos2d::Rect getObjectRect(const cocos2d::ValueMap &properties) const;
	bool getOptionalBool(const cocos2d::ValueMap &properties, const std::string &name) const;
	float getRandomFloat(float from, float to) const;

	cocos2d::Vector<cocos2d::Sprite*> m_goats;
	cocos2d::Vector<cocos2d::Sprite*> m_plants;
	mutable std::mt19937 m_random;
};
