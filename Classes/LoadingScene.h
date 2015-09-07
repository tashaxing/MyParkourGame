#pragma once
/*
//游戏预加载类
*/
#include "cocos2d.h"
class LoadingScene : public cocos2d::Scene
{
public:
	virtual bool init() override;
	CREATE_FUNC(LoadingScene);
private:
	virtual void onEnter() override;
	void loadingCallBack(cocos2d::Texture2D *texture); //加载回调函数

private:
	cocos2d::ProgressTimer *loadProgress; //进度条
	int totalNum; //总共要加载的资源
	int loadedNum; //已经加载的资源数
};