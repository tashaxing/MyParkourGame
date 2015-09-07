#pragma once
/*
//游戏角色类
*/
#include "cocos2d.h"
#include "common.h"
using namespace cocos2d;

class Player:public Node
{
public:
	virtual bool init() override;
	CREATE_FUNC(Player);
public:
	void run(); //主角奔跑
	void jump(); //主角跳跃
	void slide(); //主角滑行
	PlayerState playerState; //角色状态
private:
	Sprite *playerSprite; //奔跑的角色精灵
	Sprite *playerSpriteSlideJump; //滑行和起跳的角色精灵
	Animate *playerAnim;
	Texture2D *jumpTexture;
	Texture2D *slideTexture;
	
};
