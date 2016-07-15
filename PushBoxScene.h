#include "cocos2d.h"

USING_NS_CC;

class PushBoxScene :public Layer
{
public:
	PushBoxScene();
	~PushBoxScene();
	static cocos2d::Scene* createScene();
	CREATE_FUNC(PushBoxScene);

	virtual bool init();


	void initTouchEvent();



	void onRightPressed(Ref* sender);
	void onLeftPressed(Ref* sender);
	void onUpPressed(Ref* sender);
	void onDownPressed(Ref* sender);

	bool if_win(Ref* sender);
	void show_win(Ref* sender);

private:
	Size visibleSize;

	int step_count = 0;
	int least_step_count = 999;
	cocos2d::Label* label_win;
	cocos2d::Label* label_step;

	Sprite* player;
	Vec2 player_position;


	Sprite* box1;
	Sprite* box2;
	Vec2 box1_position;
	Vec2 box2_position;

	Sprite* destination1;
	Sprite* destination2;
	Sprite* destination3;
	Sprite* destination4;

	Vec2 destination1_position;
	Vec2 destination2_position;

	cocos2d::Vector<Sprite*> wall_vector;
	//cocos2d::Vector<Sprite*> box_vector;
};
