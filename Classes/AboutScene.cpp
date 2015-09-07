#include "AboutScene.h"
#include "MainMenu.h"

bool AboutScene::init()
{
	if(!Scene::init())
		return false;

	auto visibleSize=Director::getInstance()->getVisibleSize();
	auto visibleOrigin=Director::getInstance()->getVisibleOrigin();
	//添加动态背景
	auto backGround=Sprite::create("about.jpg");
	backGround->setPosition(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height/2);
	auto repeatAnim=MoveBy::create(3.0f,Vec2(0,50));
	backGround->runAction(RepeatForever::create(Sequence::create(repeatAnim,repeatAnim->reverse(),NULL)));
	this->addChild(backGround,0);

	//添加关于告示
	auto aboutLabel=Sprite::create("aboutLabel.png");
	aboutLabel->setPosition(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height/2);
	this->addChild(aboutLabel,1);
	auto text=LabelBMFont::create("author:tashaxing\nE-mail:tashaxing@163.com\n\nWish you have fun!","bitmapFontChinese.fnt");
	text->setPosition(visibleOrigin.x+visibleSize.width/2,visibleOrigin.y+visibleSize.height/2);
	this->addChild(text,2);
	
	//添加返回button
	auto backItem=MenuItemImage::create("back_btn.png","back_btn_press.png",[](Object *sender)
	{
		//用lambda表达式作为菜单函数回调
		auto mainMenu=MainMenu::createScene();
		TransitionScene *transition=TransitionFade::create(1.0f,mainMenu);
		Director::getInstance()->replaceScene(transition);
	});
	backItem->setPosition(visibleOrigin.x+backItem->getContentSize().width/2,visibleOrigin.y+visibleSize.height-backItem->getContentSize().height/2);
	auto backMenu=Menu::createWithItem(backItem);
	backMenu->setPosition(Point::ZERO);
	
	this->addChild(backMenu,1);
	

	return true;
}