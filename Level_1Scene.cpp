//#include "ChooseLevelScene.h"
#include "HelloWorldScene.h"
#include"Level_1Scene.h"
#include"HuanRun.h"

#include<iostream>

int static TimeNum;
char C_GetScore[20];//所得分数用到
const static float TORAD = 3.1415926 / 180.f;
static int i = 60;

USING_NS_CC;

Scene* Level_1Scene::createScence(){
	auto scene = Scene::create();
	auto layer = Level_1Scene::create();

	scene->addChild(layer);
	return scene;
}

bool Level_1Scene::init() {
	if (!Layer::init()){
		return false;
	}
	TotalScore = 0;
	pTime = 90;

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	initTouchEvent();
	//CCTexture2D *texture = CCTextureCache::sharedTextureCache()->addImage("game_bg.jpg");
	//CCSprite *bgsprite = Sprite::createWithTexture(texture);
	//添加背景
	auto bgsprite = Sprite::create("game_bg.png");
	float odds;
	float oddsY;
	oddsY = bgsprite->getContentSize().height / visibleSize.height;
	odds = bgsprite->getContentSize().width / visibleSize.width;
	bgsprite->setScaleY(1 / oddsY);
	bgsprite->setScaleX(1 / odds);
	bgsprite->setPosition(Vec2(visibleSize / 2) + origin);
	addChild(bgsprite);

	Winsize = CCDirector::sharedDirector()->getWinSize();
	huanRun = new HuanRun();
	huanRun->autorelease();
	huanRun->setPosition(ccp(Winsize.width / 2 - 2,Winsize.height / 2 + 200));
	this->addChild(huanRun);
	huanRun->runRotateAction();
	if (huanRun->getPosition().x <= huanRun->getContentSize().width /2) {
	huanRun->runToPoint(ccp(Winsize.width / 2 - 2, Winsize.height / 2 + 200));
	huanRun->runRotateAction();
	}

	setTouchEnabled(true);
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level_1Scene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level_1Scene::onTouchMoved, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//添加人
	support = CCSprite::create("7.png");
	support->setPosition(visibleSize.width / 2, 0+support->getContentSize().height/2);
	addChild(support);


	//摆动拉环
	//auto game_hand = Sprite::create("game_hand.png");
	//game_hand->setPosition(ccp(visibleSize.width / 2 - 2, visibleSize.height / 2+60));
   // addChild(game_hand);
	//auto m_biS_catch = false;
	this->schedule(schedule_selector(Level_1Scene::gameLogic), 1.0);
	this->schedule(schedule_selector(Level_1Scene::gameLogic1), 3.0);


	GoldUnitArray = CCArray::create();
	GoldUnitArray->retain();

	boUnitArray = CCArray::create();
	boUnitArray->retain();
	
	schedule(schedule_selector(Level_1Scene::checkupHip), 0.02);//检测碰撞

	schedule(schedule_selector(Level_1Scene::update), 0.02);//检测碰撞


	//this->addItemWithType(Item_gold, Gold_i);
	//this->addItemWithType(Item_stone, store_i);
	//this->addItemWithType(Item_diamond, Diamond_i);

	//时间倒退效果

	//GameTimer * m_timer = GameTimer::createTimer(90);
	//m_timer->setPosition(100, 500);
	//this->addChild(m_timer);


	label1 = Label::create("", "fonts/Marker Felt.ttf", 40);
	label1->setPosition(30, Winsize.height-30);
	this->addChild(label1);
	schedule(schedule_selector(Level_1Scene::update1));//计分

	label = Label::create("","fonts/Marker Felt.ttf",40);
	label->setPosition(Winsize.width-30, Winsize.height-30);
	this->addChild(label);
	schedule(schedule_selector(Level_1Scene::update2));//记时

    //pos = huanRun->getPosition();//开始点
	//rotateAngle = 0.f; //旋转的弧度
	//addAngle = 1.f*TORAD; //每次旋转的弧度

	//ScoreRun();

	auto backItem = MenuItemImage::create("back_normal.png", "back_clicked.png", CC_CALLBACK_1(Level_1Scene::ReturnMain, this));
	backItem->setPosition(origin + Vec2(visibleSize.width, 0) - Vec2(backItem->getContentSize().width, -backItem->getContentSize().height));
	auto menuback = Menu::create(backItem, NULL);
	menuback->setPosition(Vec2::ZERO);
	this->addChild(menuback, 1);

	return true;
}

void Level_1Scene::update1(float dt) {
	TotalScore += dt;
	char*str = new char[10];
	sprintf(str, "%d", (int)TotalScore);
	label1->setString(str);
}
void Level_1Scene::update2(float dt) {
	pTime -= dt;
	if (pTime < 1) {
		auto label2 = Label::createWithTTF("", "fonts/Marker Felt.ttf", 50);
		label2->setColor(ccc3(255, 255, 225));
		label2->setPosition(Winsize.width / 2, Winsize.height / 2);
		addChild(label2, 88);
		char*str2 = new char[10];
		sprintf(str2, "Time up!! Your TotalScore Is %d", (int)TotalScore);
		label2->setString(str2);
		Director::getInstance()->pause();

	}
	char*str1 = new char[10];
	sprintf(str1, "%d", (int)pTime);
	label->setString(str1);

}
void Level_1Scene::gameLogic1(float dt) {
	this->addbo();
}
void Level_1Scene::initTouchEvent(){
	Size visibleSize = Director::getInstance()->getVisibleSize();
	auto menu = Menu::create();
	menu->setPosition(visibleSize.width, 0);
	menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	this->addChild(menu, 10);

	
	//auto label_W = Label::createWithTTF("W", "fonts/Marker Felt.ttf", 100);
	auto label_S = Label::createWithTTF("S", "fonts/Marker Felt.ttf", 50);
	auto label_A = Label::createWithTTF("A", "fonts/Marker Felt.ttf", 50);
	auto label_D = Label::createWithTTF("D", "fonts/Marker Felt.ttf", 50);

	//auto button_up = MenuItemLabel::create(label_W, CC_CALLBACK_1(HelloWorld::onUpPressed, this));
	auto button_down = MenuItemLabel::create(label_S, CC_CALLBACK_1(Level_1Scene::onDownPressed, this));
	auto button_left = MenuItemLabel::create(label_A, CC_CALLBACK_1(Level_1Scene::onLeftPressed, this));
	auto button_right = MenuItemLabel::create(label_D, CC_CALLBACK_1(Level_1Scene::onRightPressed, this));

	float SIZE_WSAD = 100.0;
	//button_up->setPosition(SIZE_WSAD * -1, SIZE_WSAD * 2);
	button_down->setPosition(SIZE_WSAD * -1, SIZE_WSAD * 0);
	button_left->setPosition(SIZE_WSAD * -2, SIZE_WSAD * 1);
	button_right->setPosition(SIZE_WSAD * 0, SIZE_WSAD * 1);

	//button_up->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	button_down->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	button_left->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	button_right->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

	//menu->addChild(button_up);
	menu->addChild(button_down);
	menu->addChild(button_left);
	menu->addChild(button_right);
}

void Level_1Scene::onLeftPressed(Ref* ref){
	Vec2 r = Vec2(support->getPosition().x - 64, support->getPosition().y);
	Vec2 rr = Vec2(support->getPosition().x + 128, support->getPosition().y);

	if (r.x >= 0 && r.x <= Winsize.width) {
		support->setPosition(r);
	}
}

void Level_1Scene::onRightPressed(Ref* ref){
	Vec2 r = Vec2(support->getPosition().x + 64, support->getPosition().y);
	Vec2 rr = Vec2(support->getPosition().x + 128, support->getPosition().y);

	if (r.x >= 0 && r.x <= Winsize.width) {
		support->setPosition(r);
	}
}
void Level_1Scene::onDownPressed(Ref* ref){
	p = huanRun->getPosition();
	if (p.x == Winsize.width / 2 - 2 && p.y == Winsize.height / 2 + 200){
		huanRun->runTargetAction();
	}
	//return true;
}
bool Level_1Scene::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	
	return true;
}

void Level_1Scene::onTouchMoved(Touch *touch, CCEvent *pEvent)
{
	
}

void Level_1Scene::ccTouchEnded(CCSet *pTouch, CCEvent *pEvent)
{

}


void Level_1Scene::checkupHip(float dt) {

}

void Level_1Scene::fun(CCNode *node){
	removeChild(node);
	ItemUnitArray->removeObject(node);
}

void Level_1Scene::gameLogic(float dt) {
	this->addTarget();
}

void Level_1Scene::addbo() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *bo = CCSprite::create("bombicon.png");


	//int actualY = rand() % rangeY + minY;
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int actualY = random(0, int(visibleSize.width));
	//target->setPosition(ccp(Winsize.width / 2 - 2, Winsize.height / 2 + 60));
	//target->setPosition(ccp(winSize.width - target->getContentSize().width / 2.0, actualY));
	bo->setPosition(ccp(actualY, Winsize.height / 2+50));
	//bo->setPosition(ccp(actualY, Winsize.width / 2 - 30));
	bo->setTag(1);
	///bo->setTag(2);
	if (0 < bo->getPosition().x && bo->getPosition().x < winSize.width / 2 - 50 || winSize.width/2 + 50 < bo->getPosition().x &&bo->getPosition().x < winSize.width){
		this->addChild(bo);
		//this->addChild(bo);
		//GoldUnitArray->addObject(target);
		boUnitArray->addObject(bo);

		//随机速度
		//float minDuration = 2.0;
		////float maxDuration = 4.0;
		//int rangeDuration = maxDuration - minDuration;
		//float actualDuration = rand() % rangeDuration + minDuration;

		//CCFiniteTimeAction *actionMove = CCMoveTo::create(10, ccp(actualY, 0 - target->getContentSize().width / 2.0));
		CCFiniteTimeAction *actionMove1 = CCMoveTo::create(6, ccp(actualY, 0 - bo->getContentSize().height / 2.0));
		//回收屏幕外的精灵
		CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(Level_1Scene::spriteMoveFinished));
		//target->runAction(CCSequence::create(actionMove, actionMove1, actionMoveDone, NULL));
		//target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
		bo->runAction(CCSequence::create(actionMove1, actionMoveDone, NULL));
	}
}

void Level_1Scene::addTarget() {
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *target = CCSprite::create("mine_gold_b_1.png");
	
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		int actualY = random(0, int(visibleSize.width));
		//target->setPosition(ccp(Winsize.width / 2 - 2, Winsize.height / 2 + 60));
		//target->setPosition(ccp(winSize.width - target->getContentSize().width / 2.0, actualY));
		target->setPosition(ccp(actualY, Winsize.height / 2+50));
		//bo->setPosition(ccp(actualY, Winsize.width / 2 - 30));
		target->setTag(1);
		///bo->setTag(2);
		if (0 < target->getPosition().x && target->getPosition().x < winSize.width / 2 - 50 || winSize.width/2 + 50 < target->getPosition().x &&target->getPosition().x < winSize.width){
		this->addChild(target);
		//this->addChild(bo);
		GoldUnitArray->addObject(target);
		//boUnitArray->addObject(bo);
		

		CCFiniteTimeAction *actionMove = CCMoveTo::create(10, ccp(actualY, 0 - target->getContentSize().width / 2.0));
		//CCFiniteTimeAction *actionMove1 = CCMoveTo::create(10, ccp(actualY, 0 - bo->getContentSize().width / 2.0));
		//回收屏幕外的精灵
		CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(Level_1Scene::spriteMoveFinished));
		//target->runAction(CCSequence::create(actionMove, actionMove1, actionMoveDone, NULL));
		target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
		//bo->runAction(CCSequence::create(actionMove1, actionMoveDone, NULL));
	}

}

void Level_1Scene::spriteMoveFinished(cocos2d::CCNode *sender){
	CCSprite *sprite = (CCSprite *)sender;
	this->removeChild(sprite, true);  
	if (sprite->getTag() == 1) {
		GoldUnitArray->removeObject(sprite);//清除敌人
		boUnitArray->removeObject(sprite);
	}
}

void Level_1Scene::update(float dt){
	int score = 10;
	for (int i = 0; i < GoldUnitArray->count(); i++) {
		CCSprite *target = (CCSprite *)GoldUnitArray->objectAtIndex(i);
		if (target->boundingBox().intersectsRect(huanRun->boundingBox())) {
			GoldUnitArray->removeObjectAtIndex(i);
			this->removeChild(target);
			update1(10);
			huanRun->runToPoint(ccp(Winsize.width / 2 - 2, Winsize.height / 2 + 200));
			huanRun->runRotateAction();
			break;
		}
		if (target->boundingBox().intersectsRect(support->boundingBox())) {
		
			GoldUnitArray->removeObjectAtIndex(i);
			this->removeChild(target);
			update1(10);
		}
		
	}
	for (int i = 0; i < boUnitArray->count(); i++) {
		CCSprite *bo = (CCSprite *)boUnitArray->objectAtIndex(i);
		if (bo->boundingBox().intersectsRect(huanRun->boundingBox())) {
			auto label_S = Label::createWithTTF("you lost", "fonts/Marker Felt.ttf", 50);
			label_S->setColor(ccc3(255, 255, 225));
			label_S->setPosition(Winsize.width / 2, Winsize.height / 2);
			addChild(label_S, 88);
			Director::getInstance()->pause();
			
			break;
		}
		if (bo->boundingBox().intersectsRect(support->boundingBox())) {
			auto label_S = Label::createWithTTF("you lost", "fonts/Marker Felt.ttf", 50);
			label_S->setColor(ccc3(255, 255, 225));
			label_S->setPosition(Winsize.width / 2, Winsize.height / 2);
			addChild(label_S, 88);
			Director::getInstance()->pause();
			
		}

	}
}

//void Level_1Scene::draw() {

//}
void Level_1Scene::ReturnMain(Ref *ref){
	//this->stopAllActions();
	auto Scene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(Scene);
}

bool Level_1Scene::iscollision(CCSprite*sprite1, CCSprite*sprite2){
	CCRect rect = sprite2->boundingBox();
	if (rect.containsPoint(sprite1->getPosition()))
	{
		return true;
	}
	return false;
}

void Level_1Scene::ScoreRun()
{

	sprintf(C_GetScore, "%d", TotalScore);
	CCLOG(C_GetScore, "%d", TotalScore);//测试
	NumScore->setString(C_GetScore);

}