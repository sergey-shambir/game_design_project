#pragma once

#include "AbstractScene.h"

class WelcomeScene : public AbstractScene
{
public:
	static cocos2d::Scene *createScene();

	void tryInit() override;

	// implement the "static create()" method manually
	CREATE_FUNC(WelcomeScene);
};
