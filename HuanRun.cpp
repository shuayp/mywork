#include "HuanRun.h"

HuanRun::HuanRun()
{
	CCSprite::initWithFile("game_hand.png");//����һ���ҹ�

	isCatchItem = false;//�����ֵ��Ϊfalse
}
void HuanRun::runRotateAction()
{
	this->setAnchorPoint(CCPointMake(0.5, 2));//�趨ê��

	CCActionInterval* action1 = CCRotateTo::create(2, -90);//�ùҹ�������������ת90

	CCActionInterval* action2 = CCRotateTo::create(2, 90);//�ùҹ�������������ת90

	float rotate = this->getRotation();//��һ��Folat��¼һ��ֵ

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

	    float x = 0;//��x��ʼ��Ϊ0

	    float y = 0;//��y��ʼ��Ϊ0

	    float rotation = this->getRotation();

	    if (rotation>0) {
	        if(rotation <= 56)//56�ǶԽ������봹ֱ���γɵĽǣ���ֱ�߼����е�Ĵ�ֱ��
	        {
	            y = 0.0;
	            x = 240 - 160*tan(rotation*3.1415/180);//*3.1415/180����˼�ǰѽǶ�ת���ɻ���
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
	    this->runToPoint(ccp(x, y));//�ó�x��y

}

void HuanRun::runToPoint( CCPoint point )
{
	this->stopAllActions();//ֹͣһ�ж���

	CCPoint start = this->getPosition();//��ȡֹͣʱ�ҹ���λ��

	float distance = sqrtf((start.x-point.x)*(start.x-point.x) +  (start.y-point.y)*(start.y-point.y));

	float time = distance * 0.01;

	CCActionInterval* actionTo = CCMoveTo::create(time, point);

	CCCallFunc* fun  = CCCallFunc::create(this, callfunc_selector(HuanRun::runBack));

	CCSequence* seq = CCSequence::create(actionTo, fun, NULL);//������Ķ�����˳��ִ��

	this->runAction(seq);//���
}

void HuanRun::runBack()
{
    CCSize Winsize = CCDirector::sharedDirector()->getWinSize();//��ȡ��Ļ��С
	

	CCActionInterval* action4 = CCMoveTo::create(2,ccp(Winsize.width/2-2,Winsize.height/2+200));//���÷���λ��

	CCCallFunc* func = CCCallFunc::create(this, callfunc_selector(HuanRun::runRotateAction));

	CCSequence* SequenceZon = CCSequence::create(action4, func, NULL);//���������ж�����˳�򲥷�

	this->runAction(SequenceZon);//���


}

