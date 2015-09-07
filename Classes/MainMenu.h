#pragma once
/*
//游戏主菜单类
*/

#include "cocos2d.h"
USING_NS_CC;

class MainMenu: public Layer
{
public:
	static Scene *createScene();

	CREATE_FUNC(MainMenu);
private:
	//菜单回调
	void menuStartCallback(Ref *sender);
	void menuOptionCallback(Ref *sender);
	void menuAboutCallback(Ref *sender);

	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
};