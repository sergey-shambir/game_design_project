#include "ViewsFactory.h"

using namespace cocos2d;

namespace
{
std::string getDefaultFontName()
{
#if 1
	return "fonts/Neucha.ttf";
#else
	return "fonts/arial.ttf";
#endif
}

std::string getTitleFontName()
{
	return getDefaultFontName();
}

TTFConfig getMenuFont()
{
	return TTFConfig{ getDefaultFontName(), 20 };
}

TTFConfig getIndicatorFont()
{
	return TTFConfig{ getDefaultFontName(), 28 };
}

TTFConfig getTitleFont()
{
	return TTFConfig{ getTitleFontName(), 40 };
}

Color3B getButtonTextColor()
{
	return Color3B(51, 51, 51);
}

float getMenuItemDistance()
{
	return 20;
}

int getButtonFontSize()
{
	return 36;
}

cocos2d::RefPtr<Label> createLabel(const std::string &textUtf8, const TTFConfig &font)
{
	RefPtr<Label> label = Label::createWithTTF(font, textUtf8, TextHAlignment::CENTER);
	if (!label)
	{
		throw std::runtime_error("cannot create label with text \"" + textUtf8 + "\"");
	}
	return label;
}
} // namespace

cocos2d::RefPtr<Label> ViewsFactory::createLargeLabel(const std::string &textUtf8)
{
	return createLabel(textUtf8, getMenuFont());
}

cocos2d::RefPtr<Label> ViewsFactory::createIndicatorLabel(const std::string &textUtf8)
{
	const Color3B color = getButtonTextColor();
	auto label = createLabel(textUtf8, getIndicatorFont());
	label->setTextColor(Color4B{ color.r, color.g, color.b, 255 });
	return label;
}

cocos2d::RefPtr<Label> ViewsFactory::createTitleLabel(const std::string &textUtf8)
{
	return createLabel(textUtf8, getTitleFont());
}

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

cocos2d::RefPtr<Menu> ViewsFactory::createVerticalMenuWithArray(const cocos2d::Vector<MenuItem *> &items)
{
	auto menu = cocos2d::Menu::createWithArray(items);
	for (size_t i = 0, size = items.size(); i < size; ++i)
	{
		const float offsetRatio = 0.5f * static_cast<float>(size - 1) - static_cast<float>(i);
		const float offset = offsetRatio * getMenuItemDistance();
		items.at(i)->setPosition(Vec2{ 0, offset });
	}
	if (!menu)
	{
		throw std::runtime_error("cannot create menu");
	}
	return menu;
}

RefPtr<ui::Button> ViewsFactory::createButton(const std::string &title, const std::function<void()> &onClick)
{
	ui::Button *button = ui::Button::create("res/btn-normal.png", "res/btn-selected.png", "res/btn-disabled.png");
	if (!button)
	{
		throw std::runtime_error("cannot create button '" + title + "'");
	}

	button->setTitleText(title);
	button->setTitleColor(getButtonTextColor());
	button->setTitleFontName(getDefaultFontName());
	button->setTitleFontSize(getButtonFontSize());
	button->addClickEventListener([onClick](Ref *sender) {
		(void)sender;
		onClick();
	});

	return button;
}

RefPtr<LayerColor> ViewsFactory::createColoredLayer(const Color4B &color, const Size &size)
{
	RefPtr<LayerColor> layer = LayerColor::create(color, size.width, size.height);
	if (!layer)
	{
		throw std::runtime_error("cannot create colored layer");
	}
	return layer;
}
