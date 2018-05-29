#include "GameLevelMap.h"

using namespace cocos2d;

namespace
{
const char groundLayerName[] = "ground_tiles";
const char unitsLayerName[] = "units";
const char goatObjectType[] = "goat";
const char plantObjectType[] = "plant";

void dumpValueMapKeys(const ValueMap &map)
{
	std::string keysStr;
	for (auto &&pair : map)
	{
		if (!keysStr.empty())
		{
			keysStr += ", ";
		}
		keysStr += pair.first;
		switch (pair.second.getType())
		{
		case Value::Type::BOOLEAN:
			keysStr += pair.second.asBool() ? ": true" : ": false";
			break;
		case Value::Type::FLOAT:
			keysStr += ": " + std::to_string(pair.second.asFloat());
			break;
		case Value::Type::DOUBLE:
			keysStr += ": " + std::to_string(pair.second.asDouble());
			break;
		case Value::Type::INTEGER:
			keysStr += ": " + std::to_string(pair.second.asInt());
			break;
		case Value::Type::UNSIGNED:
			keysStr += ": " + std::to_string(pair.second.asUnsignedInt());
			break;
		case Value::Type::STRING:
			keysStr += ": " + pair.second.asString();
			break;
		}
	}
	CCLOG("keys: %s", keysStr.c_str());
}
} // namespace

GameLevelMap *GameLevelMap::create(const std::string &tmxFile)
{
	RefPtr<GameLevelMap> ret = new (std::nothrow) GameLevelMap;
	ret->initWithTMXFile(tmxFile);
	ret->autorelease();
	return ret;
}

cocos2d::Size GameLevelMap::getMapVisibleSize() const
{
	Size sizeInTiles = TMXTiledMap::getMapSize();
	Size tileSize = TMXTiledMap::getTileSize();

	return Size{ tileSize.width * sizeInTiles.width, tileSize.height * sizeInTiles.height };
}

void GameLevelMap::initWithTMXFile(const std::string &tmxFile)
{
	if (!TMXTiledMap::initWithTMXFile(tmxFile))
	{
		throw std::runtime_error("cannot parse map from " + tmxFile);
	}
	const float scaleFactor = Director::getInstance()->getContentScaleFactor();
	TMXTiledMap::setScale(scaleFactor);

	loadUnits();
}

void GameLevelMap::loadUnits()
{
	TMXObjectGroup *group = getObjectGroupOrThrow(unitsLayerName);
	dumpValueMapKeys(group->getProperties());
	for (Value object : group->getObjects())
	{
		ValueMap props = object.asValueMap();
		dumpValueMapKeys(props);
		Value gidPropsValue = getPropertiesForGID(props.at("gid").asInt());
		if (gidPropsValue.getType() != Value::Type::NONE)
		{
			ValueMap gidProps = gidPropsValue.asValueMap();
			dumpValueMapKeys(gidProps);
		}
	}
}

TMXObjectGroup *GameLevelMap::getObjectGroupOrThrow(const std::string &name)
{
	TMXObjectGroup *group = TMXTiledMap::getObjectGroup(name);
	if (!group)
	{
		throw std::runtime_error("can't find layer '" + name + "' on map");
	}
	return group;
}
