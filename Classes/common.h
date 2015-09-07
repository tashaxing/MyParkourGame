#pragma once
/*
//公共类型和变量头文件
*/

//游戏关卡枚举
enum Level
{
	SPRING,
	WINTER
};


//游戏角色枚举
enum PlayerType
{
	BOY,
	GIRL
};

//游戏角色状态
enum PlayerState
{
	RUN,
	SLIDE,
	JUMP
};

//地图元素枚举
enum BlockType
{
	LAND,  //砖块
	NPC,   //怪物
	STAR,  //星星
	TOOL,  //工具
	NONE   //空
};

//场景间隔基本单位定为80，分辨率800*480，则排满是10*6个格子，便于作碰撞检测
const float BLOCK_SIZE=80.0f; 
const float PICKUP_SIZE=40.0f;
const float PLAYER_RADIUS=50.0f;
const float GRAVITY=-1500.0f;
const float PLAYER_SPEED=700.0f;

