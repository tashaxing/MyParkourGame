#include "Player.h"


extern PlayerType playerType; //用的全局变量来存储游戏角色，后面全部改成配置文件

bool Player::init()
{
	if(!Node::init())
		return false;
	//定义男孩和女孩的序列帧动画,注意此处要经常切换纹理，所以不能用AnimationCache
	Animation *girlAnimation=Animation::create();
	for(int i=1;i<=8;i++)
		girlAnimation->addSpriteFrameWithFile("girl"+std::to_string(i)+".png");
	girlAnimation->setDelayPerUnit(0.15f);

	Animation *boyAnimation=Animation::create();
	for(int i=1;i<=12;i++)
		boyAnimation->addSpriteFrameWithFile("boy"+std::to_string(i)+".png");
	boyAnimation->setDelayPerUnit(0.1f);

	//选取不同角色
	std::string playerTextureName;
	std::string playerJumpTexureName;
	std::string playerSlideTextureName;
	Animation *playerAnimation;

	//添加游戏角色，分男女
	switch(playerType)
	{
	case BOY:
		playerTextureName="boy1.png";
		playerJumpTexureName="boy_jump.png";
		playerSlideTextureName="boy_slide.png";
		playerAnimation=boyAnimation;
		break;
	case GIRL:
		playerTextureName="girl1.png";
		playerJumpTexureName="girl_jump.png";
		playerSlideTextureName="girl_slide.png";
		playerAnimation=girlAnimation;
		break;
	}

	

	playerSprite=Sprite::create(playerTextureName); //此处必须初始化一张角色纹理，否则后面无法切换纹理
	
	jumpTexture=Sprite::create(playerJumpTexureName)->getTexture(); //创建跳跃纹理
	slideTexture=Sprite::create(playerSlideTextureName)->getTexture(); //创建滑行纹理

	playerAnim=Animate::create(playerAnimation);
	this->addChild(playerSprite);

	auto playerBody=PhysicsBody::createBox(playerSprite->getContentSize()); //这里要用包围盒，如果用圆形的话会导致滚动
	playerBody->setDynamic(true);
	playerBody->setContactTestBitmask(1);
	playerBody->setGravityEnable(true);
	playerBody->getShape(0)->setRestitution(0.0f); //设置刚体回弹力
	this->setPhysicsBody(playerBody);

	playerSprite->runAction(RepeatForever::create(playerAnim)); //初始时刻角色在奔跑
	playerState=RUN;



	return true;
}

void Player::run()
{
	playerState=RUN;
	playerSprite->resume();
}

void Player::jump()
{
	playerState=JUMP;
	
	playerSprite->pause();
	playerSprite->setTexture(jumpTexture); //设置跳跃的纹理
	this->getPhysicsBody()->setVelocity(Vec2(0,PLAYER_SPEED));

}

void Player::slide()
{
	playerState=SLIDE;
	playerSprite->pause();
	playerSprite->setTexture(slideTexture); //设置滑行的纹理
}