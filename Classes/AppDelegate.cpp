#include "AppDelegate.h"
#include "LoadingScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) 
	{
        glview = GLView::create("My Parkour Game");
		glview->setFrameSize(800,480); //设置屏幕尺寸
        director->setOpenGLView(glview);
    }

	glview->setDesignResolutionSize(800,480,ResolutionPolicy::EXACT_FIT); //设置分辨率适配，这里设置的是设计分辨率
    
	//设置默认路径，这样就不用使用images/bird.png这样的分目录了，直接用bird.png就行的
	std::vector<std::string> resPaths;
	resPaths.push_back("fonts");
	resPaths.push_back("images");
	resPaths.push_back("sounds");
	FileUtils::getInstance()->setSearchResolutionsOrder(resPaths);
	
	// turn off display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene=LoadingScene::create(); //这里使用的是默认的create函数

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
