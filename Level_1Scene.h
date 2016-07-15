#include "cocos2d.h"
#include"HuanRun.h"
USING_NS_CC;

class Level_1Scene : public cocos2d::Layer
{
public:
	Level_1Scene();
	~Level_1Scene();
	static cocos2d::Scene* createScence(); 

	virtual bool onTouchBegan(cocos2d::Touch *touch, CCEvent *pEvent);

	static bool iscollision(Sprite*spritel, Sprite*sprite2);
	//void addItemWithType(ItemKindType type, int count);//��ӵ����ͺ�����
	virtual bool init();


	void update(float dt);
	void update1(float dt);

	//void addItem();
	void ReturnMain(Ref *ref);
	void fun(CCNode * node);//ɾ����Ʒ
	void gameLogic(float dt);
	void gameLogic1(float dt);
	void addTarget();
	void addbo();
	void spriteMoveFinished(CCNode *sender);

	void initTouchEvent();
	void onLeftPressed(Ref* ref);
	void onDownPressed(Ref* ref);
	void onRightPressed(Ref* ref);
	void openmenu(Ref *ref);
	void menurestart(Ref *ref);
	void menuquit(Ref *ref);
	void menuback(Ref *ref);
	void NextScene(Ref *ref);

	void ScoreRun();

	CCLabelBMFont * Num;
	HuanRun*huanRun;
	int TotalScore;

	Label* label1;
	Label* label;
	float pTime;

	CCSize Winsize;
	float rotateAngle;//����
	float addAngle;//ÿ�����ӵĻ���
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
	CCLabelBMFont *NumScore;//���÷���

	CCArray *GoldUnitArray;
	CCArray *boUnitArray;
	Level_1Scene *a;
	float ratio;
	int stateQuan;
};

