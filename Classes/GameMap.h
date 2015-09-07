#pragma once
/*
//”Œœ∑µÿÕº¿‡
*/
#include "cocos2d.h"
USING_NS_CC;


class GameMap:public Node
{
public:
	virtual bool init() override;
	CREATE_FUNC(GameMap);

public:
	void moveNode(Node *child);
private:
	void mapUpdate(float dt);
};