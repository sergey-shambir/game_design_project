#pragma once

#include "AbstractScene.h"
#include "GameLevelMap.h"

class GameLevelScene : public AbstractScene
{
public:
    static cocos2d::Scene *createScene();

    void tryInit() override;

    // implement the "static create()" method manually
    CREATE_FUNC(GameLevelScene);

private:
    void preloadResources();

	cocos2d::RefPtr<GameLevelMap> m_map;
};
