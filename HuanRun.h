#ifndef _HuanRun_H_
#define _HuanRun_H_

#include "cocos2d.h"
USING_NS_CC;
class HuanRun:public CCSprite
{
public:
	HuanRun();
	bool isCatchItem;
	void runRotateAction();//挂钩左右摆动的函数
	void runTargetAction();//计算挂钩要到的点的位置
	void runToPoint(CCPoint point);//挂钩运动的总动作
	void runBack();//挂钩返回函数
	void runMiner();//执行矿工动作函数
};

#endif//HuanRun