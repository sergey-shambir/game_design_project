#pragma once

#include "cocos2dgame.h"
#include <type_traits>

class ViewsFactory
{
public:
	ViewsFactory() = delete;

	static cocos2d::RefPtr<cocos2d::MenuItemLabel> createMenuItemLabel(const std::string &textUtf8, const cocos2d::ccMenuCallback &cb);

	template<class... Args>
	static cocos2d::RefPtr<cocos2d::Menu> createVerticalMenu(Args &&... args)
	{
		std::initializer_list<cocos2d::MenuItem*> items = { args... };
		return createVerticalMenuWithArray(items);
	}

	static cocos2d::RefPtr<cocos2d::Menu> createVerticalMenuWithArray(const cocos2d::Vector<cocos2d::MenuItem*> &items);
};
