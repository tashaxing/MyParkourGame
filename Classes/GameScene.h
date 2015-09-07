#pragma once
/*
//游戏主场景类
*/

#include "cocos2d.h"
#include "Player.h"
#include "GameMap.h"
USING_NS_CC;

class GameScene: public Layer
{
public:
	static Scene *createScene();

	CREATE_FUNC(GameScene);
private:
	virtual bool init() override;
	virtual void onEnter() override;
	virtual void onExit() override;
	//触摸检测
	virtual bool onTouchBegan(Touch *touch,Event *event) override;
	virtual void onTouchEnded(Touch *touch,Event *event) override;
	//碰撞检测
	bool onContactBegin(const PhysicsContact &contact);
	//刷新
	virtual void update(float dt) override;
private:
	void backGroundUpdate(float dt); //背景滚屏
	void addScore(float number); //得分
	void gameOver(); //游戏结束
private:
	Size visibleSize;
	Point visibleOrigin;

	Player *player; //角色

	//两个控制按钮
	Sprite *slideBtn;
	Vector<Texture2D *> slideBtnTextures;
	Sprite *jumpBtn;
	Vector<Texture2D *> jumpBtnTextures;

	Sprite *backGround1,*backGround2; //背景
	GameMap *gameMap; //地图

	int score; //分数
	LabelBMFont *scoreLabel; //分数精灵
};