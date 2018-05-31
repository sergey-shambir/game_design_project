#include "CustomEvents.h"

LevelEventData *LevelEventData::create(const std::string &levelId)
{
    auto event = new LevelEventData(levelId);
    event->autorelease();
    return event;
}

std::string LevelEventData::getLevelId() const
{
    return m_levelId;
}

LevelEventData::LevelEventData(const std::string &levelId)
    : m_levelId(levelId)
{
}
