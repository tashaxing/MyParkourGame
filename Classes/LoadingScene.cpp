#include "SimpleAudioEngine.h"
#include "LoadingScene.h"
#include "MainMenu.h"
using namespace cocos2d;
using namespace CocosDenshion;


bool LoadingScene::init()
{
	if (!Scene::init())
		return false;
	Size visibleSize=Director::getInstance()->getVisibleSize();
	Vec2 visibleOrigin=Director::getInstance()->getVisibleOrigin();
	//没有添加层，直接在场景里面添加精灵
	//添加启动画面

	auto splashBkground=Sprite::create("splash.png");
	splashBkground->setPosition(Vec2(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height/2));
	this->addChild(splashBkground);

	
	//初始化加载的资源数
	totalNum=2;
	loadedNum=0;
	
	//初始化进度条
	Sprite *progressBkground=Sprite::create("sliderTrack.png"); //进度条底图
	Size progressBackSize=progressBkground->getContentSize();
	progressBkground->setPosition(Point(visibleOrigin.x+100+progressBackSize.width/2,visibleOrigin.y+60));
	this->addChild(progressBkground);


	loadProgress=ProgressTimer::create(Sprite::create("sliderProgress.png")); //创建进度条
	loadProgress->setBarChangeRate(Vec2(1,0)); //设置横向
	loadProgress->setType(ProgressTimer::Type::BAR); //条状
	loadProgress->setMidpoint(Vec2(0,1)); //设置从左到右变化 
	Size progressSize=loadProgress->getContentSize();
	loadProgress->setPosition(Point(visibleOrigin.x+100+progressSize.width/2,visibleOrigin.y+60));
	loadProgress->setPercentage(0.0f);
	this->addChild(loadProgress);

	return true;
}

void LoadingScene::onEnter()
{
	//添加加载回调函数，异步预加载纹理
	Director::getInstance()->getTextureCache()->addImageAsync("boy.png",CC_CALLBACK_1(LoadingScene::loadingCallBack,this));
	Director::getInstance()->getTextureCache()->addImageAsync("girl.png",CC_CALLBACK_1(LoadingScene::loadingCallBack,this));

}

void LoadingScene::loadingCallBack(Texture2D *texture)
{
	loadedNum++;
	//此处的预加载帧动画指示用于实验性质
	switch(loadedNum)
	{
	case 1:
		//预加载帧缓存纹理
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("boy.plist",texture);
		loadProgress->setPercentage((float)loadedNum/totalNum*100);
		break;
	case 2:
		//预加载帧缓存纹理
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("girl.plist",texture);
		loadProgress->setPercentage((float)loadedNum/totalNum*100);
		break;
	default:
		break;
	}

	if(loadedNum==totalNum)
	{
		//预加载帧动画
		auto boyAnimation=Animation::create();
		boyAnimation->setDelayPerUnit(0.1f);
		for(int i=1;i<=12;i++)
		{
			char str[100]={0};
			sprintf(str,"boy%d.png",i);
			boyAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(str));
		}
			
		AnimationCache::getInstance()->addAnimation(boyAnimation,"boyAnimation");

		//预加载帧动画
		auto girlAnimation=Animation::create();
		girlAnimation->setDelayPerUnit(0.2f);
		for(int i=1;i<=8;i++)
			girlAnimation->addSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("girl"+std::to_string(i)+".png"));
		AnimationCache::getInstance()->addAnimation(girlAnimation,"girlAnimation");

		////预加载音乐和音效
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("spring_music.wav");
		SimpleAudioEngine::getInstance()->preloadBackgroundMusic("winter_music.mp3");

		SimpleAudioEngine::getInstance()->preloadEffect("jump.wav");
		SimpleAudioEngine::getInstance()->preloadEffect("point.mp3");
		SimpleAudioEngine::getInstance()->preloadEffect("gameover.wav");
		
		//加载完毕跳转到游戏场景
		auto mainMenu=MainMenu::createScene();
		TransitionScene *transition=TransitionFade::create(1.0f,mainMenu);
		Director::getInstance()->replaceScene(transition);
	}
	
}