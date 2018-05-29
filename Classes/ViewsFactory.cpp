#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
TTFConfig getMenuFont()
{
	return TTFConfig{ "fonts/arial.ttf", 16 };
}

float getMenuItemDistance()
{
	return 20;
}
} // namespace

cocos2d::RefPtr<cocos2d::MenuItemLabel> ViewsFactory::createMenuItemLabel(const std::string &textUtf8, const ccMenuCallback &cb)
{
	RefPtr<Label> label = Label::createWithTTF(getMenuFont(), textUtf8, TextHAlignment::CENTER);
	if (!label)
	{
		throw std::runtime_error("cannot create label with text \"" + textUtf8 + "\"");
	}
	auto item = MenuItemLabel::create(label, cb);
	if (!item)
	{
		throw std::runtime_error("cannot create menu item with text \"" + textUtf8 + "\"");
	}
	return item;
}

cocos2d::RefPtr<Menu> ViewsFactory::createVerticalMenuWithArray(const cocos2d::Vector<MenuItem*>& items)
{
	auto menu = cocos2d::Menu::createWithArray(items);
	for (size_t i = 0, size = items.size(); i < size; ++i)
	{
		const float offsetRatio = 0.5f * static_cast<float>(size - 1) - static_cast<float>(i);
		const float offset = offsetRatio * getMenuItemDistance();
		items.at(i)->setPosition(Vec2{0, offset});
	}
	if (!menu)
	{
		throw std::runtime_error("cannot create menu");
	}
	return menu;
}
