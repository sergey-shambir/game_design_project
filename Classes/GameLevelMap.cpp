#include "GameLevelMap.h"

using namespace cocos2d;
using namespace std::literals;

namespace
{
const char kLayerNameGround[] = "ground_tiles";
const char kLayerNameUnits[] = "units";
const char kPropertyType[] = "type";
const char kPropertyFlipX[] = "flipX";
const char kTypeGoat[] = "goat";
const char kTypePlant[] = "plant";
const float kGoatIdleSecondsMin = 1.5f;
const float kGoatIdleSecondsMax = 7.0f;

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
		default:
			break;
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

	std::random_device device;
	m_random.seed(device());

	loadUnits();
}

void GameLevelMap::loadUnits()
{
	TMXObjectGroup *group = getObjectGroupOrThrow(kLayerNameUnits);
	for (Value object : group->getObjects())
	{
		ValueMap properties = object.asValueMap();
		const std::string objectType = properties.at(kPropertyType).asString();
		if (objectType == kTypeGoat)
		{
			auto sprite = spawnGoat(properties);
			m_goats.pushBack(sprite);
			this->addChild(sprite);
		}
		else if (objectType == kTypePlant)
		{
			auto sprite = spawnObjectSprite("plant_bush_ripe.png", properties);
			m_plants.pushBack(sprite);
			this->addChild(sprite);
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

cocos2d::RefPtr<Sprite> GameLevelMap::spawnGoat(const ValueMap &properties)
{
	auto sprite = spawnObjectSprite("goat_idle.png", properties);
	Rect spriteRect = sprite->getTextureRect();

	SpriteFrameCache *cache = SpriteFrameCache::getInstance();
	SpriteFrame *frame1 = cache->getSpriteFrameByName("goat_idle.png");
	SpriteFrame *frame2 = cache->getSpriteFrameByName("goat_eat1.png");
	SpriteFrame *frame3 = cache->getSpriteFrameByName("goat_eat2.png");
	SpriteFrame *frame4 = cache->getSpriteFrameByName("goat_eat3.png");
	Vector<SpriteFrame *> frames = {
		frame1, frame2, frame3, frame4, frame3, frame4, frame3, frame4, frame3, frame2, frame1
	};

	Animate *animate = Animate::create(Animation::createWithSpriteFrames(frames, 0.1f));
	assert(animate);

	DelayTime *delay = DelayTime::create(getRandomFloat(kGoatIdleSecondsMin, kGoatIdleSecondsMax));
	sprite->runAction(RepeatForever::create(Sequence::createWithTwoActions(delay, animate)));

	return sprite;
}

RefPtr<Sprite> GameLevelMap::spawnObjectSprite(const std::string &frameName, const ValueMap &properties) const
{
	const Rect spriteRect = getObjectRect(properties);
	RefPtr<Sprite> sprite = Sprite::createWithSpriteFrameName(frameName);
	if (!sprite)
	{
		throw std::runtime_error("cannot load sprite \""s + frameName + "\""s);
	}
	sprite->setPosition(spriteRect.origin + 0.5f * Vec2{ spriteRect.size });
	sprite->setAnchorPoint(Vec2{ 0.5f, 0.5f });

	return sprite;
}

Rect GameLevelMap::getObjectRect(const ValueMap &properties) const
{
	Rect rect;
	rect.origin.x = properties.at("x").asFloat();
	rect.origin.y = properties.at("y").asFloat();
	rect.size.width = properties.at("width").asFloat();
	rect.size.height = properties.at("height").asFloat();

	return rect;
}

bool GameLevelMap::getOptionalBool(const ValueMap &properties, const std::string &name) const
{
	auto it = properties.find(name);
	if (it != properties.end() && !it->second.isNull())
	{
		return it->second.asBool();
	}
	return false;
}

float GameLevelMap::getRandomFloat(float from, float to) const
{
	std::uniform_real_distribution<float> distribution{ from, to };
	return distribution(m_random);
}
