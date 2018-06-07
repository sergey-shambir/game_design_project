#pragma once

#include "AbstractScene.h"
#include "HeadUpDisplayLayer.h"
#include "GameLevelMap.h"
#include "ScoreManager.h"

class GameLevelScene : public AbstractScene
{
public:
	static cocos2d::Scene *createScene(const std::string &levelId);
	static GameLevelScene *create(const std::string &levelId);

	void tryInit() override;

private:
	void preloadResources();
	void removeListeners();
	void reportWin();
	void switchNextScene();
	void switchWelcomeScene();
	RoundConditions getRoundConditions() const;

	void onEnter() override;
	void onExit() override;

	std::string m_levelId;
	cocos2d::RefPtr<GameLevelMap> m_map;
	cocos2d::RefPtr<HeadUpDisplayLayer> m_hud;
	std::vector<cocos2d::RefPtr<cocos2d::EventListenerCustom>> m_listeners;
};
