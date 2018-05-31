#pragma once

#include "AbstractScene.h"
#include "BoundariesLayer.h"
#include "GameLevelMap.h"

class GameLevelScene : public AbstractScene
{
public:
	static cocos2d::Scene *createScene(const std::string &levelId);
	static GameLevelScene *create(const std::string &levelId);

	void tryInit() override;

private:
	void preloadResources();
	void removeListeners();
	void switchNextScene();
	void switchWelcomeScene();
	void onEnter() override;
	void onExit() override;

	std::string m_levelId;
	cocos2d::RefPtr<GameLevelMap> m_map;
	cocos2d::RefPtr<BoundariesLayer> m_hud;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_winListener;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_loseListener;
	cocos2d::RefPtr<cocos2d::EventListenerCustom> m_retryListener;
};
