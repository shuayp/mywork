#include "cocos2d.h"
#include"Res.h"
#include"HuanRun.h"
USING_NS_CC;

class Level_1Scene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScence(); 
	static CCPoint getPointWithType(ItemKindType type, ItemSizeType sizeType);

	virtual bool onTouchBegan(cocos2d::Touch *touch, CCEvent *pEvent);
	virtual void onTouchMoved(cocos2d::CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCSet *pTouch, CCEvent *pEvent);

	static bool iscollision(Sprite*spritel, Sprite*sprite2);
	//void addItemWithType(ItemKindType type, int count);//添加的类型和数量
	virtual bool init();
	void checkupHip(float dt);


	void update(float dt);
	void update1(float dt);
	void update2(float dt);

	//void addItem();
	void ReturnMain(Ref *ref);
	void fun(CCNode * node);//删除物品
	void gameLogic(float dt);
	void gameLogic1(float dt);
	void addTarget();
	void addbo();
	void spriteMoveFinished(CCNode *sender);

	void initTouchEvent();
	void onLeftPressed(Ref* ref);
	void onDownPressed(Ref* ref);
	void onRightPressed(Ref* ref);

	void ScoreRun();

	CCLabelBMFont * Num;
	HuanRun*huanRun;
	int TotalScore;

	Label*               label1;
	Label*               label;
	float                   pTime;

	CCSize Winsize;
	float rotateAngle;		//弧度
	float addAngle;			//每次增加的弧度
	CCArray *ItemUnitArray;
	CREATE_FUNC(Level_1Scene);

private:
	Sprite * game_hand;
	CCSprite *support;
	Point game_hand_p;
	CCPoint p;
	Point offset;
	Point point;
	Point pos;
	CCLabelBMFont *NumScore;//所得分数

	CCArray *GoldUnitArray;
	CCArray *boUnitArray;
	Level_1Scene *a;
	float ratio;
	int stateQuan;
};

