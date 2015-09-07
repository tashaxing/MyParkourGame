#include "GameMap.h"
#include "common.h"

extern Level level;

bool GameMap::init()
{
	if(!Node::init())
		return false;

	//添加砖块
	std::string block_file;
	std::string npc_file;

	switch(level)
	{
	case SPRING:
		block_file="block_spring.png";
		npc_file="lvshuiling.png";
		break;
	case WINTER:
		block_file="block_winter.png";
		npc_file="lanmogu.png";
		break;
	}

	std::string star_file="star.png";
	std::string tool_file="accelerate.png";

	//手动搭建地图，没有加入即时计算~
	//1层
	for(int i=0;i<10;i++)
	{
		if(i!=3&&i!=4&&i!=7&&i!=8)
		{
			//添加land
			auto block=Sprite::create(block_file);
			block->setPosition(BLOCK_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+1*BLOCK_SIZE);
			this->addChild(block);
			block->setTag(LAND); //设置tag
			auto blockBody=PhysicsBody::createBox(block->getContentSize());
			blockBody->setDynamic(false);
			blockBody->setContactTestBitmask(1);
			blockBody->getShape(0)->setRestitution(0);
			block->setPhysicsBody(blockBody);
		}
	}
	//2层
	for(int i=0;i<10;i++)
	{
		if(i==2||i==5||i==6)
		{
			//添加怪物
			auto npc=Sprite::create(npc_file);
			npc->setTag(NPC);
			npc->setPosition(BLOCK_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+2*BLOCK_SIZE);
			auto npcBody=PhysicsBody::createBox(npc->getContentSize());
			npcBody->setDynamic(false);
			npcBody->setContactTestBitmask(1);
			npcBody->getShape(0)->setRestitution(0);
			npc->setPhysicsBody(npcBody);
			this->addChild(npc);
		}
		if(i==3)
		{
			//添加land
			auto block=Sprite::create(block_file);
			block->setPosition(BLOCK_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+1*BLOCK_SIZE);
			this->addChild(block);
			block->setTag(LAND); //设置tag
			auto blockBody=PhysicsBody::createBox(block->getContentSize());
			blockBody->setDynamic(false);
			blockBody->setContactTestBitmask(1);
			blockBody->getShape(0)->setRestitution(0);
			block->setPhysicsBody(blockBody);
		}
	}
	//3层
	for(int i=0;i<10;i++)
	{
		if(i!=0&&i!=3&&i!=4)
		{
			//添加星星
			auto star1=Sprite::create(star_file);
			star1->setTag(STAR);
			star1->setPosition(PICKUP_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+3*BLOCK_SIZE);
			auto starBody1=PhysicsBody::createBox(star1->getContentSize());
			starBody1->setDynamic(false);
			starBody1->setContactTestBitmask(1);
			starBody1->getShape(0)->setRestitution(0.0f);
			star1->setPhysicsBody(starBody1);
			this->addChild(star1);

			auto star2=Sprite::create(star_file);
			star2->setTag(STAR);
			star2->setPosition(PICKUP_SIZE/2*3+i*BLOCK_SIZE,BLOCK_SIZE/2+3*BLOCK_SIZE);
			auto starBody2=PhysicsBody::createBox(star2->getContentSize());
			starBody2->setDynamic(false);
			starBody2->setContactTestBitmask(1);
			starBody2->getShape(0)->setRestitution(0.0f);
			star2->setPhysicsBody(starBody2);
			this->addChild(star2);

		}

	}
	//4层
	for(int i=0;i<10;i++)
	{
		if(i==3||i==4)
		{
			//添加land
			auto block=Sprite::create(block_file);
			block->setPosition(BLOCK_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+4*BLOCK_SIZE);
			this->addChild(block);
			block->setTag(LAND); //设置tag
			auto blockBody=PhysicsBody::createBox(block->getContentSize());
			blockBody->setDynamic(false);
			blockBody->setContactTestBitmask(1);
			blockBody->getShape(0)->setRestitution(0);
			block->setPhysicsBody(blockBody);

		}
		if(i==8)
		{
			auto star1=Sprite::create(star_file);
			star1->setTag(STAR);
			star1->setPosition(PICKUP_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+3*BLOCK_SIZE);
			auto starBody1=PhysicsBody::createBox(star1->getContentSize());
			starBody1->setDynamic(false);
			starBody1->setContactTestBitmask(1);
			starBody1->getShape(0)->setRestitution(0.0f);
			star1->setPhysicsBody(starBody1);
			this->addChild(star1);

			auto star2=Sprite::create(star_file);
			star2->setTag(STAR);
			star2->setPosition(PICKUP_SIZE/2*3+i*BLOCK_SIZE,BLOCK_SIZE/2+3*BLOCK_SIZE);
			auto starBody2=PhysicsBody::createBox(star2->getContentSize());
			starBody2->setDynamic(false);
			starBody2->setContactTestBitmask(1);
			starBody2->getShape(0)->setRestitution(0.0f);
			star2->setPhysicsBody(starBody2);
			this->addChild(star2);
		}
		if(i==6)
		{
			//添加道具
			auto tool=Sprite::create(tool_file);
			tool->setTag(TOOL);
			tool->setPosition(PICKUP_SIZE/2+i*BLOCK_SIZE,BLOCK_SIZE/2+3*BLOCK_SIZE);
			auto toolBody=PhysicsBody::createBox(tool->getContentSize());
			toolBody->setDynamic(false);
			toolBody->setContactTestBitmask(1);
			toolBody->getShape(0)->setRestitution(0.0f);
			tool->setPhysicsBody(toolBody);
			this->addChild(tool);
		}
	}


	//启动调度器，地图滚屏
	this->schedule(schedule_selector(GameMap::mapUpdate),0.01f);

	return true;
}

void GameMap::mapUpdate(float dt)
{
	for(auto &node:this->getChildren())
	{
		node->setPositionX(node->getPositionX()-3.0f);
		if(node->getPositionX()<=-node->getContentSize().width/2)
			node->setPositionX(node->getPositionX()+BLOCK_SIZE/2+10*BLOCK_SIZE);
	}

}

void GameMap::moveNode(Node *child)
{
	child->setPositionX(child->getPositionX()+BLOCK_SIZE/2+10*BLOCK_SIZE);
}