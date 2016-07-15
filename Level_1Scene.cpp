#include "HelloWorldScene.h"
#include"Level_1Scene.h"
#include"HuanRun.h"
#include "SecondScene.h"
#include "SimpleAudioEngine.h"
#include<iostream>

int static TimeNum;
char C_GetScore[20];//所得分数用到
const static float TORAD = 3.1415926 / 180.f;

USING_NS_CC;

Level_1Scene::Level_1Scene() {
}

Level_1Scene::~Level_1Scene() {
}

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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_music.wma");

	TotalScore = 0;

	//添加菜单
	auto menuitem = MenuItemFont::create("Menu", CC_CALLBACK_1(Level_1Scene::openmenu, this));
	menuitem->setColor(ccc3(255,255,255));
	auto menu = Menu::create();
	menu->addChild(menuitem);
	menu->setPosition(menuitem->getContentSize().width / 2, visibleSize.height - menuitem->getContentSize().height / 2);
	addChild(menu, 2);


	initTouchEvent();
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

	//添加圆环
	Winsize = CCDirector::sharedDirector()->getWinSize();
	huanRun = new HuanRun();
	huanRun->autorelease();
	huanRun->setPosition(ccp(Winsize.width / 2 - 2,Winsize.height / 2 + 200));
	this->addChild(huanRun);
	huanRun->runRotateAction();

	setTouchEnabled(true);

	//设置监听事件
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->onTouchBegan = CC_CALLBACK_2(Level_1Scene::onTouchBegan, this);
	touchListener->onTouchMoved = CC_CALLBACK_2(Level_1Scene::onTouchMoved, this);
	dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

	//添加人
	support = CCSprite::create("7.png");
	support->setPosition(visibleSize.width / 2, 0+support->getContentSize().height/2);
	addChild(support);

	//设置金币和炸弹的出现
	this->schedule(schedule_selector(Level_1Scene::gameLogic), 1.0);
	this->schedule(schedule_selector(Level_1Scene::gameLogic1), 3.0);


	GoldUnitArray = CCArray::create();
	GoldUnitArray->retain();

	boUnitArray = CCArray::create();
	boUnitArray->retain();
	
	schedule(schedule_selector(Level_1Scene::checkupHip), 0.02);//检测碰撞
	schedule(schedule_selector(Level_1Scene::update), 0.02);//检测碰撞


	label1 = Label::create("", "fonts/Marker Felt.ttf", 40);
	label1->setPosition(Winsize.width/2, Winsize.height-30);
	this->addChild(label1);
	schedule(schedule_selector(Level_1Scene::update1));//计分


    pos = huanRun->getPosition();//开始点
	rotateAngle = 0.f; //旋转的弧度
	addAngle = 1.f*TORAD; //每次旋转的弧度

	return true;
}


//左上角的菜单栏
void Level_1Scene::openmenu(Ref *ref) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto restartitem = MenuItemFont::create("Restart", CC_CALLBACK_1(Level_1Scene::menurestart, this));
	auto quititem = MenuItemFont::create("Quit", CC_CALLBACK_1(Level_1Scene::menuquit, this));
	auto backitem = MenuItemFont::create("Back", CC_CALLBACK_1(Level_1Scene::menuback, this));
	restartitem->setColor(ccc3(0, 0, 0));
	quititem->setColor(ccc3(0, 0, 0));
	backitem->setColor(ccc3(0, 0, 0));

	auto menu = Menu::create();
	menu->addChild(backitem);
	menu->addChild(restartitem);
	menu->addChild(quititem);
	menu->alignItemsVerticallyWithPadding(restartitem->getContentSize().height / 2 + 10);
	menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(menu, 10);
}

//菜单键返回
void Level_1Scene::menuback(Ref *ref) {
	auto startscene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(startscene);
}

//菜单键重新开始
void Level_1Scene::menurestart(Ref *ref)
{
	auto SecondScene = Level_1Scene::createScence();
	Director::getInstance()->replaceScene(SecondScene);
}


//菜单键退出
void Level_1Scene::menuquit(Ref *ref) {
	Director::getInstance()->end();
}


//更新分数并显示
void Level_1Scene::update1(float dt) {
	TotalScore += dt;
	char*str = new char[10];
	sprintf(str, "%d", (int)TotalScore);
	label1->setString(str);
}


//跳到下一个关卡
void Level_1Scene::NextScene(Ref *ref) {
	auto Scene = SecondScene::createScene();
	Director::getInstance()->replaceScene(Scene);
}

void Level_1Scene::gameLogic1(float dt) {
	this->addbo();
}

void Level_1Scene::gameLogic(float dt) {
	this->addTarget();
}


//添加控制键
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

void Level_1Scene::fun(CCNode *node){
	removeChild(node);
	ItemUnitArray->removeObject(node);
}

//随机位置添加炸弹
void Level_1Scene::addbo() {
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *bo = CCSprite::create("bombicon.png");


	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	int actualY = random(0, int(visibleSize.width));
	bo->setPosition(ccp(actualY, Winsize.height / 2+50));
	bo->setTag(1);
	if (0 < bo->getPosition().x && bo->getPosition().x < winSize.width / 2 - 50 || winSize.width/2 + 50 < bo->getPosition().x &&bo->getPosition().x < winSize.width){
		this->addChild(bo);
		boUnitArray->addObject(bo);

		CCFiniteTimeAction *actionMove1 = CCMoveTo::create(6, ccp(actualY, 0 - bo->getContentSize().height / 2.0));
		//回收屏幕外的精灵
		CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(Level_1Scene::spriteMoveFinished));

		bo->runAction(CCSequence::create(actionMove1, actionMoveDone, NULL));
	}
}

//随机位置添加金币
void Level_1Scene::addTarget() {
	
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *target = CCSprite::create("mine_gold_b_1.png");
	
		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		int actualY = random(0, int(visibleSize.width));
		target->setPosition(ccp(actualY, Winsize.height / 2+50));
		target->setTag(1);
		if (0 < target->getPosition().x && target->getPosition().x < winSize.width / 2 - 50 || winSize.width/2 + 50 < target->getPosition().x &&target->getPosition().x < winSize.width){
		this->addChild(target);
		GoldUnitArray->addObject(target);
		

		CCFiniteTimeAction *actionMove = CCMoveTo::create(10, ccp(actualY, 0 - target->getContentSize().width / 2.0));
		//回收屏幕外的精灵
		CCFiniteTimeAction *actionMoveDone = CCCallFuncN::create(this, callfuncN_selector(Level_1Scene::spriteMoveFinished));
		target->runAction(CCSequence::create(actionMove, actionMoveDone, NULL));
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

//检测碰撞，如果碰到金币则加分，如果遇到炸弹，则结束游戏，并显示跳到下一关
void Level_1Scene::update(float dt){
	int score = 10;
	for (int i = 0; i < GoldUnitArray->count(); i++) {
		CCSprite *target = (CCSprite *)GoldUnitArray->objectAtIndex(i);
		if (target->boundingBox().intersectsRect(huanRun->boundingBox())) {
			GoldUnitArray->removeObjectAtIndex(i);
			this->removeChild(target);
			huanRun->runToPoint(ccp(Winsize.width / 2 - 2, Winsize.height / 2 + 200));
			huanRun->runRotateAction();
			update1(10);
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
			this->unscheduleAllSelectors();
			auto label2 = Label::createWithTTF("", "fonts/Marker Felt.ttf", 50);
			label2->setColor(ccc3(255, 255, 225));
			label2->setPosition(Winsize.width / 2, Winsize.height / 2);
			addChild(label2, 88);
			char*str2 = new char[10];
			sprintf(str2, "Your TotalScore Is %d", (int)TotalScore);
			label2->setString(str2);

			auto item1 = MenuItemLabel::create(Label::createWithSystemFont("Next Level", "fonts/Marker Felt.ttf", 30), CC_CALLBACK_1(Level_1Scene::NextScene, this));
			auto menu = Menu::create(item1, NULL);
			menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + 40);
			menu->alignItemsVerticallyWithPadding(30);
			this->addChild(menu, 1);

			this->stopAllActions();
			
			break;
		}
		if (bo->boundingBox().intersectsRect(support->boundingBox())) {
			this->unscheduleAllSelectors();
			auto label2 = Label::createWithTTF("", "fonts/Marker Felt.ttf", 50);
			label2->setColor(ccc3(255, 255, 225));
			label2->setPosition(Winsize.width / 2, Winsize.height / 2);
			addChild(label2, 88);
			char*str2 = new char[10];
			sprintf(str2, "Your TotalScore Is %d", (int)TotalScore);
			label2->setString(str2);

			auto item1 = MenuItemLabel::create(Label::createWithSystemFont("Next Level", "fonts/Marker Felt.ttf", 30), CC_CALLBACK_1(Level_1Scene::NextScene, this));
			auto menu = Menu::create(item1, NULL);
			menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + 40);
			menu->alignItemsVerticallyWithPadding(30);
			this->addChild(menu, 1);

			this->stopAllActions();
			/*auto label_S = Label::createWithTTF("you lost", "fonts/Marker Felt.ttf", 50);
			this->unscheduleAllSelectors();
			label_S->setColor(ccc3(255, 255, 225));
			label_S->setPosition(Winsize.width / 2, Winsize.height / 2);
			addChild(label_S, 88);*/
			//Director::getInstance()->pause();
			
		}
	}
}

void Level_1Scene::ReturnMain(Ref *ref){
	this->stopAllActions();
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
	NumScore->setString(C_GetScore);

}