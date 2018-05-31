#pragma once

#include "IGameLevelMap.h"
#include <random>

class GameLevelMap
	: public cocos2d::TMXTiledMap,
	  public IGameLevelMap
{
public:
	static GameLevelMap *create(const cocos2d::Size &layerSize, const std::string &levelId);

	cocos2d::Size getMapVisibleSize() const;

	std::string getLevelId() const final;
	unsigned getBoundaryCount() const final;
	std::vector<cocos2d::Rect> getAllObstacles() const final;
	std::vector<cocos2d::Rect> getAnimalsRects() const final;
	std::vector<cocos2d::Rect> getPlantsRects() const final;

private:
	void initWithTMXFile(const cocos2d::Size &layerSize, const std::string &levelId);
	void loadUnits();
	cocos2d::TMXObjectGroup *getObjectGroupOrThrow(const std::string &name);
	cocos2d::RefPtr<cocos2d::Sprite> spawnGoat(const cocos2d::ValueMap &properties) const;
	cocos2d::RefPtr<cocos2d::Sprite> spawnObjectSprite(const std::string &frameName, const cocos2d::ValueMap &properties) const;
	std::vector<cocos2d::Rect> getMapSpritesRects(const cocos2d::Vector<cocos2d::Sprite *> &sprites) const;

	cocos2d::Rect getObjectRect(const cocos2d::ValueMap &properties) const;
	bool getOptionalBool(const cocos2d::ValueMap &properties, const std::string &name) const;
	float getRandomFloat(float from, float to) const;

	std::string m_levelId;
	cocos2d::Vector<cocos2d::Sprite *> m_goats;
	cocos2d::Vector<cocos2d::Sprite *> m_plants;
	unsigned m_boundaryCount = 0;
	mutable std::mt19937 m_random;
};
