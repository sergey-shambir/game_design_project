#pragma once

#include "AbstractScene.h"
#include "BoundariesLayer.h"
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

	void update(float delta) override;
	void onEnter() override;
	void onExit() override;

	std::string m_levelId;
	cocos2d::RefPtr<GameLevelMap> m_map;
	cocos2d::RefPtr<BoundariesLayer> m_hud;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_winListener;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_loseListener;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_retryListener;

	float m_secondsLeft = 0;
};
