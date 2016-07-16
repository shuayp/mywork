#ifndef __SecondScene_H__
#define __SecondScene_H__

#include "cocos2d.h"
#include <string>
USING_NS_CC;

class SecondScene : public cocos2d::Layer
{
public:
	SecondScene();
	~SecondScene();
	static cocos2d::Scene* createScene();
	static SecondScene *create(PhysicsWorld *world);
	virtual bool init(PhysicsWorld *world);
	void ifcontact();
	bool onContactBegan(PhysicsContact& contact);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unuused_event);
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event);

	void addsprite();
	void addmonster();

	void openmenu(Ref *ref);
	void menurestart(Ref *ref);
	void menuquit(Ref *ref);
	void menuback(Ref *ref);
	void addblock(std::string blockname);
	int getScore();
	void updateScore(float dt);
	void nextScene(Ref *ref);
	void ifwin();

private:
	Size visibleSize;
	Vec2 origin;
	int score;
	int contact_count;
	Label *score_label;
	Label *time_label;
	
	int count = 0;
	float time;
	//support为支撑板，ball为小球
	Sprite *support, *ball;
	TMXTiledMap *monster;
	Map<int, Sprite*> monmap;
};

#endif
