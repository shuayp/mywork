#ifndef _HuanRun_H_
#define _HuanRun_H_

#include "cocos2d.h"
USING_NS_CC;
class HuanRun:public CCSprite
{
public:
	HuanRun();
	bool isCatchItem;
	void runRotateAction();//�ҹ����Ұڶ��ĺ���
	void runTargetAction();//����ҹ�Ҫ���ĵ��λ��
	void runToPoint(CCPoint point);//�ҹ��˶����ܶ���
	void runBack();//�ҹ����غ���
	void runMiner();//ִ�п󹤶�������
};

#endif//HuanRun