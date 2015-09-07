#include "SimpleAudioEngine.h"
#include "common.h"
#include "MainMenu.h"
#include "GameScene.h"
#include "GameMap.h"

using namespace CocosDenshion;

//初始化全局变量
extern Level level=SPRING;
extern PlayerType playerType=BOY;
extern bool isSound=true;

int jumpTimes=0; //当前跳跃次数
int jumpTotal=2; //最多跳跃次数

Scene *GameScene::createScene()
{
	//初始化化游戏物理场景
	auto *scene=Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0,GRAVITY));
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL); //调试模式看包围盒
	//初始化主layer
	auto *layer=GameScene::create();
	scene->addChild(layer);
	return scene;
}

bool GameScene::init()
{
	if(!Layer::init())
		return false;

	//获取屏幕大小及原点位置
	visibleSize=Director::getInstance()->getVisibleSize();
	visibleOrigin=Director::getInstance()->getVisibleOrigin();

	//添加背景
	std::string backGroundFile;
	switch(level)
	{
	case SPRING:
		backGroundFile="back_spring.png";
		break;
	case WINTER:
		backGroundFile="back_winter.png";
		break;
	}
	backGround1=Sprite::create(backGroundFile);
	backGround1->setAnchorPoint(Point::ZERO);
	backGround1->setPosition(Point::ZERO);
	this->addChild(backGround1,0);
	backGround2=Sprite::create(backGroundFile);
	backGround2->setAnchorPoint(Point::ZERO);
	backGround2->setPosition(Point::ZERO);
	this->addChild(backGround2,0);

	//设置地图,默认锚点在左下角
	gameMap=GameMap::create();
	gameMap->setPosition(visibleOrigin.x,visibleOrigin.y);
	this->addChild(gameMap,1);



	//添加player
	player=Player::create();
	player->setPosition(Point(visibleOrigin.x+2*BLOCK_SIZE,visibleOrigin.y+4*BLOCK_SIZE));
	this->addChild(player,1);


	//添加滑行和跳跃按钮的事件
	score=0; //初始化分数

	slideBtn=Sprite::create("slideButton.png");
	auto slideBtnTexture1=Sprite::create("slideButton.png")->getTexture();
	auto slideBtnTexture2=Sprite::create("slideButtonPress.png")->getTexture();
	slideBtnTextures.pushBack(slideBtnTexture1);
	slideBtnTextures.pushBack(slideBtnTexture2);
	slideBtn->setScale(0.5);
	slideBtn->setPosition(Point(visibleOrigin.x+100,visibleOrigin.y+50));
	this->addChild(slideBtn,2);

	jumpBtn=Sprite::create("jumpButton.png");
	auto jumpBtnTexture1=Sprite::create("jumpButton.png")->getTexture();
	auto jumpBtnTexture2=Sprite::create("jumpButtonPress.png")->getTexture();
	jumpBtnTextures.pushBack(jumpBtnTexture1);
	jumpBtnTextures.pushBack(jumpBtnTexture2);
	jumpBtn->setScale(0.5);
	jumpBtn->setPosition(Point(visibleOrigin.x+visibleSize.width-100,visibleOrigin.y+50));
	this->addChild(jumpBtn,2);

	//添加基本UI，分数等
	scoreLabel=LabelBMFont::create("score: ","bitmapFontChinese.fnt");
	scoreLabel->setPosition(visibleOrigin.x+visibleSize.width-150,visibleOrigin.y+visibleSize.height-20);
	this->addChild(scoreLabel,2);
	scoreLabel->setString(String::createWithFormat("score: %d",score)->_string);


	this->scheduleUpdate();//启动默认更新
	this->schedule(schedule_selector(GameScene::backGroundUpdate),0.02f); //启动背景循环滚屏

	//添加碰撞监测
	auto contactListener=EventListenerPhysicsContact::create();
	contactListener->onContactBegin=CC_CALLBACK_1(GameScene::onContactBegin,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener,this);

	//添加触摸监听
	auto touchListener=EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan=CC_CALLBACK_2(GameScene::onTouchBegan,this);
	touchListener->onTouchEnded=CC_CALLBACK_2(GameScene::onTouchEnded,this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener,this);

	return true;
}

bool GameScene::onTouchBegan(Touch *touch,Event *event)
{
	auto touchPoint=touch->getLocation();
	//检测是否触摸在按钮区域
	if(slideBtn->getBoundingBox().containsPoint(touchPoint))
	{
		slideBtn->setTexture(slideBtnTextures.at(1));
		player->slide();
	}

	if(jumpTimes<jumpTotal&&jumpBtn->getBoundingBox().containsPoint(touchPoint))
	{
		if(isSound)
			SimpleAudioEngine::getInstance()->playEffect("jump.wav"); //播放跳跃音效
		jumpBtn->setTexture(jumpBtnTextures.at(1));
		player->jump();
		jumpTimes++;
	}
		
	return true;
}

void GameScene::onTouchEnded(Touch *touch,Event *event)
{
	auto touchPoint=touch->getLocation();
	//判断是释放时是否在按钮区域
	if(slideBtn->getBoundingBox().containsPoint(touchPoint))
	{
		slideBtn->setTexture(slideBtnTextures.at(0));
		player->run();
	}

	if(jumpBtn->getBoundingBox().containsPoint(touchPoint))
	{
		jumpBtn->setTexture(jumpBtnTextures.at(0));
	}

}

void GameScene::update(float dt)
{
	//在此处做碰撞检测，如果前一个状态是跳跃则落地后继续奔跑
	

	//防止角色因为碰撞发生旋转
	player->setRotation(0.0f); 
		
	//当角色被挡道之后跟上原来的位置
	float step=2.0f;
	if(player->getPositionX()<2*BLOCK_SIZE)
		player->setPositionX(player->getPositionX()+step);
	if(player->getPositionX()>2*BLOCK_SIZE)
		player->setPositionX(player->getPositionX()-step);

	//判断游戏结束
	if(player->getPositionY()<=0.0f)
		gameOver();
}

bool GameScene::onContactBegin(const PhysicsContact &contact)
{
	jumpTimes=0; //落回地面就将已跳跃次数清零

	//获得被碰撞物体，getShapeA  getShapeB要尝试一下
	auto target=contact.getShapeA()->getBody()->getNode();
	if(target->getTag()==STAR)
	{
		//碰到星星就涨分，星星消失
		gameMap->moveNode(target);
		addScore(100); //拾取星星得100
	}
	else if(target->getTag()==NPC&&target->getPositionY()+target->getContentSize().height/2<player->getPositionY()) //此处要用else if,只有当角色在怪物头上才能踩中
	{
		gameMap->moveNode(target);
		addScore(150); //踩怪得150
	}
	else if(target->getTag()==NPC&&target->getPositionY()+target->getContentSize().height/2>=player->getPositionY()) //如果角色正面遇到怪物就挂了
		gameOver();
	else if(target->getTag()==TOOL)
	{
		jumpTotal=3;
		auto toolIcon=Sprite::create("accelerate_state.png");
		toolIcon->setPosition(Point(visibleOrigin.x+180,visibleOrigin.y+50));
		this->addChild(toolIcon,2);
		target->removeFromParent(); //道具只出现一次，从parent里面删除
		addScore(300); //道具300
	}

	//落回地面恢复跑步状态
	if(player->playerState==JUMP)
		player->run();

	return true;
}

void GameScene::onEnter()
{
	Layer::onEnter();
	//进入场景播放音乐
	if(isSound)
		SimpleAudioEngine::getInstance()->playBackgroundMusic(level==SPRING?"spring_music.wav":"winter_music.mp3",true);
}

void GameScene::onExit()
{
	Layer::onExit();
	//退出场景关闭音乐
	if(SimpleAudioEngine::getInstance()->isBackgroundMusicPlaying())
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void GameScene::backGroundUpdate(float dt)
{
	backGround1->setPositionX(backGround1->getPositionX()-1.0f);
	backGround2->setPositionX(backGround1->getPositionX()+backGround1->getContentSize().width);
	if(backGround2->getPositionX()<=0.0f)
		backGround1->setPositionX(0.0f);
}

void GameScene::addScore(float number)
{
	if(isSound)
		SimpleAudioEngine::getInstance()->playEffect("point.mp3"); //播放得分音效
	score+=number;
	scoreLabel->setString(String::createWithFormat("score: %d",score)->_string);
}

void GameScene::gameOver()
{
	//游戏结束停止所有的调度器
	gameMap->unscheduleAllSelectors();
	this->unscheduleAllSelectors();
	//播放游戏结束声音
	if(isSound)
		SimpleAudioEngine::getInstance()->playEffect("gameover.wav");

	//游戏结束出现菜单
	visibleSize=Director::getInstance()->getVisibleSize();
	visibleOrigin=Director::getInstance()->getVisibleOrigin();
	
	auto gameOverPanel=Node::create();
	auto overLabel=Sprite::create("gameover.png");
	overLabel->setPosition(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height/2+100);
	gameOverPanel->addChild(overLabel);

	auto backItem=MenuItemImage::create("back_to_menu.png","back_to_menu_press.png",[](Object *sender)
	{
		//用lambda表达式作为菜单函数回调
		auto mainMenu=MainMenu::createScene();
		TransitionScene *transition=TransitionFade::create(1.0f,mainMenu);
		Director::getInstance()->replaceScene(transition);
	});
	auto backMenu=Menu::createWithItem(backItem);
	backMenu->setPosition(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height/2-50);

	gameOverPanel->addChild(backMenu);

	gameOverPanel->setPositionY(visibleOrigin.y+visibleSize.height);
	this->addChild(gameOverPanel,3);
	//滑入gameover logo，注意node的锚点在左下角
	gameOverPanel->runAction(MoveTo::create(0.5f,Vec2(visibleOrigin.x,visibleOrigin.y)));

}