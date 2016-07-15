#include "HuanRun.h"

HuanRun::HuanRun()
{
	CCSprite::initWithFile("game_hand.png");//创建一个挂钩

	isCatchItem = false;//将存的值变为false
}
void HuanRun::runRotateAction()
{
	this->setAnchorPoint(CCPointMake(0.5, 2));//设定锚点

	CCActionInterval* action1 = CCRotateTo::create(2, -90);//让挂钩在两秒内向左转90

	CCActionInterval* action2 = CCRotateTo::create(2, 90);//让挂钩在两秒内向右转90

	float rotate = this->getRotation();//用一个Folat记录一个值

	CCSequence* action3;

	if(rotate>0)
		{
			action3 = CCSequence::create(action1, action2, NULL);
		}
		else
		{
			action3 = CCSequence::create(action2, action1, NULL);
		}

		CCRepeatForever* repeat = CCRepeatForever::create(action3);

		this->runAction(repeat);
}

void HuanRun::runTargetAction()
{

	    float x = 0;//将x初始化为0

	    float y = 0;//将y初始化为0

	    float rotation = this->getRotation();

	    if (rotation>0) {
	        if(rotation <= 56)//56是对角线与与垂直线形成的角，垂直线即是中点的垂直线
	        {
	            y = 0.0;
	            x = 240 - 160*tan(rotation*3.1415/180);//*3.1415/180的意思是把角度转化成弧度
	        }
	        else
	        {
	            y = 160 - 240*tan((90-rotation)*3.1415/180);
	            x = 0.0;
	        }
	    }
	    else if(rotation<0)
	    {
	        if(rotation<= -56)
	        {
	            y=160- 240*tan((90-abs(rotation))*3.1415/180);
	            x= 480;
	        }
	        else
	        {
	            y=0;
	            x= 240+160*tan(abs(rotation)*3.1415/180);
	        }
	    }
	    else if(rotation == 0)
	    {
	        x= 240;
	        y= 160;
	    }
		if (x < 0) x = 0;
		if (x > 480) x = 480;
		if (y < 0) y = 0;
	    this->runToPoint(ccp(x, y));//得出x，y

}

void HuanRun::runToPoint( CCPoint point )
{
	this->stopAllActions();//停止一切动作

	CCPoint start = this->getPosition();//获取停止时挂钩的位置

	float distance = sqrtf((start.x-point.x)*(start.x-point.x) +  (start.y-point.y)*(start.y-point.y));

	float time = distance * 0.01;

	CCActionInterval* actionTo = CCMoveTo::create(time, point);

	CCCallFunc* fun  = CCCallFunc::create(this, callfunc_selector(HuanRun::runBack));

	CCSequence* seq = CCSequence::create(actionTo, fun, NULL);//让上面的动作摁顺序执行

	this->runAction(seq);//添加
}

void HuanRun::runBack()
{
    CCSize Winsize = CCDirector::sharedDirector()->getWinSize();//获取屏幕大小
	

	CCActionInterval* action4 = CCMoveTo::create(2,ccp(Winsize.width/2-2,Winsize.height/2+200));//设置返回位置

	CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(HuanRun::runRotateAction));

	CCSequence* SequenceZon = CCSequence::create(action4, func, NULL);//将上面所有动作按顺序播放

	this->runAction(SequenceZon);//添加


}

