#pragma once

#include "cocos2dgame.h"

static constexpr char EVENT_WIN_ON_LEVEL[] = "level_win";
static constexpr char EVENT_LOSE_ON_LEVEL[] = "level_lose";

static constexpr char GAME_LEVEL1_ID[] = "res/goats_level1.tmx";
static constexpr char GAME_LEVEL2_ID[] = "res/goats_level2.tmx";
static constexpr char GAME_LEVEL3_ID[] = "res/goats_level3.tmx";
static constexpr char GAME_LEVEL_DEBUG_ID[] = "res/debug_layer.tmx";

class ICustomEventData : public cocos2d::Ref
{
public:
    virtual ~ICustomEventData() = default;
};

class LevelEventData : public ICustomEventData
{
public:
    static LevelEventData *create(const std::string &levelId);

    std::string getLevelId() const;

private:
    LevelEventData(const std::string &levelId);

    const std::string m_levelId;
};

class CustomEvents
{
public:
    CustomEvents() = delete;

    template<class T>
    static T *get(cocos2d::EventCustom *event)
    {
        ICustomEventData *data = static_cast<ICustomEventData *>(event->getUserData());
        return dynamic_cast<T*>(data);
    }

    static cocos2d::RefPtr<cocos2d::EventCustom> make(const std::string &name, ICustomEventData *data = nullptr)
    {
        cocos2d::RefPtr<cocos2d::EventCustom> event = new cocos2d::EventCustom(name);
        event->autorelease();
        event->setUserData(data);
        return event;
    }
};
