#pragma once
#include "GameStatus.h"
#include "cocos2dgame.h"
#include "IGameLevelMap.h"

class GameOverLayer : public cocos2d::DrawNode
{
public:
	static GameOverLayer *create(const cocos2d::Size& contentSize, IGameLevelMap& map, GameStatus status);
	bool init(const cocos2d::Size& contentSize, IGameLevelMap& map, GameStatus status);

private:
	IGameLevelMap* m_map = nullptr;

};
