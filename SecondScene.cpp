#include "SecondScene.h"
#include "SimpleAudioEngine.h"
#include "PushBoxScene.h"
#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "Level_1Scene.h"

USING_NS_CC;
using namespace CocosDenshion;
int i = 1;

SecondScene::SecondScene() {
	score = 500;
	time = 30.0;
}

SecondScene::~SecondScene() {
	delete monster;
	monmap.clear();
}

Scene* SecondScene::createScene()
{
	auto *scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	auto layer = SecondScene::create(scene->getPhysicsWorld());
	scene->addChild(layer);
	return scene;
}

//创建物理世界
SecondScene* SecondScene::create(PhysicsWorld *world) {
	SecondScene *pret = new SecondScene();
	if (pret && pret->init(world)) {
		return pret;
	}
	pret = NULL;
	return NULL;
}

bool SecondScene::init(PhysicsWorld *world)
{
	if (!Layer::init())
	{
		return false;
	}

	if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_music.wma");


	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	//添加背景
	auto background = Sprite::create("background1.png");
	background->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	addChild(background);

	contact_count = 0;
	//添加分数栏
	score_label = Label::createWithTTF("", "fonts/Marker Felt.ttf", 25);
	score_label->setPosition(visibleSize.width / 2, visibleSize.height- 60);
	score_label->setContentSize(Size(50,50));
	score_label->setColor(ccc3(0, 0, 0));
	score_label->retain();
	addChild(score_label);
	schedule(schedule_selector(SecondScene::updateScore), 1.0f);


	//添加支撑板，小球以及边界
	addsprite();

	//添加动物精灵
	addmonster();

	auto menuitem = MenuItemFont::create("Menu", CC_CALLBACK_1(SecondScene::openmenu, this));
	menuitem->setColor(ccc3(0, 0, 0));
	auto menu = Menu::create();
	menu->addChild(menuitem);
	menu->setPosition(menuitem->getContentSize().width / 2, visibleSize.height - menuitem->getContentSize().height / 2);
	addChild(menu, 10);

	//检测碰撞
	ifcontact();
/*
	if (!CocosDenshion::SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("game_music.wma");*/


	//设置触摸事件
	setTouchEnabled(true);
	EventListenerTouchOneByOne *tlistener = EventListenerTouchOneByOne::create();
	tlistener->setSwallowTouches(true);
	tlistener->onTouchBegan = CC_CALLBACK_2(SecondScene::onTouchBegan, this);
	tlistener->onTouchMoved = CC_CALLBACK_2(SecondScene::onTouchMoved, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(tlistener, this);

	return true;
}

//检测碰撞
void SecondScene::ifcontact(){
	//监听是否有碰撞事件发生
	auto listener = EventListenerPhysicsContact::create();
	listener->onContactBegin = CC_CALLBACK_1(SecondScene::onContactBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}


bool SecondScene::onContactBegan(PhysicsContact& contact) {
	//获取碰撞的两个物体及其tag
	auto sp1 = contact.getShapeA()->getBody()->getNode();
	auto sp2 = contact.getShapeB()->getBody()->getNode();
	if (sp1 == NULL || sp2 == NULL)
		return false;
	//如果是小球和动物的碰撞，播放音效，则将该动物移除，并从map里面删除
	if (sp1->getTag() == 0 && sp2->getTag()) {
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("4708.mp3", false, 0.1, 0.1);
		auto find = monmap.find(sp2->getTag());
		monster->removeChild(find->second);
		//updateScore(10);
		monmap.erase(find->first);
		contact_count++;
		count--;
		ifwin();
	}
	else if (sp2->getTag() == 0 && sp1->getTag()) {
		//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("4708.mp3", false, 0.1, 0.1);
		auto find = monmap.find(sp1->getTag());
		monster->removeChild(find->second);
		//updateScore(10);
		monmap.erase(find->first);
		contact_count++;
		count--;
		ifwin();
	}
	return true;
}


bool SecondScene::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *unused_event) {
	return true;
}

void SecondScene::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *unuused_event) {
	Point touchLocation = this->convertToWorldSpace(this->convertTouchToNodeSpace(touch));
	//如果移动到过高或过低的地方，将不再往上
	if (touchLocation.y >= visibleSize.height - support->getContentSize().height / 2-1) {
		touchLocation.y = visibleSize.height - support->getContentSize().height / 2 - 1;
	}
	else if (touchLocation.y <= support->getContentSize().height/2) {
		touchLocation.y = support->getContentSize().height / 2 + 1;
	}
	//在同一y轴上运动
	support->setPositionY(touchLocation.y);
}

void SecondScene::addsprite() {
	//添加边界
	auto body = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 0.0f);
	auto edgeNode = Node::create();
	edgeNode->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	edgeNode->setPhysicsBody(body);
	edgeNode->getPhysicsBody()->setDynamic(false);
	edgeNode->getPhysicsBody()->getShape(0)->setRestitution(1.0f);
	edgeNode->getPhysicsBody()->getShape(0)->setFriction(0.0f);
	edgeNode->getPhysicsBody()->getShape(0)->setDensity(0.0f);
	addChild(edgeNode);

	//添加长条
	support = Sprite::create("sup.png");
	support->setContentSize(Size(support->getContentSize().width, support->getContentSize().height));
	support->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	support->setPhysicsBody(PhysicsBody::createBox(Size(support->getContentSize().width, support->getContentSize().height)));
	support->setTag(-1);
	support->getPhysicsBody()->setDynamic(false);
	support->getPhysicsBody()->getShape(0)->setFriction(0.0f);
	support->getPhysicsBody()->getShape(0)->setRestitution(1.0f);
	support->getPhysicsBody()->getShape(0)->setDensity(0.0f);
	addChild(support);

	//添加小球
	ball = Sprite::create("10002.png");
	ball->setContentSize(Size(ball->getContentSize().width, ball->getContentSize().height));
	ball->setPosition(visibleSize.width / 2, visibleSize.height / 2 - support->getContentSize().width / 2);
	ball->setPhysicsBody(PhysicsBody::createCircle(ball->getContentSize().height / 2));
	ball->getPhysicsBody()->getShape(0)->setRestitution(1.0f);
	ball->getPhysicsBody()->getShape(0)->setFriction(0.0f);
	ball->getPhysicsBody()->getShape(0)->setDensity(0.0f);
	//给小球一个力
	Vect force = Vect(350.0f, 350.0f);
	ball->getPhysicsBody()->applyImpulse(force);
	addChild(ball);
	ball->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
	ball->setTag(0);
}

void SecondScene::addmonster() {
	//添加其他精灵
	monster = TMXTiledMap::create("things.tmx");
	monster->setPosition(0, 0);
	monster->setAnchorPoint(Vec2(0.5, 0.5));
	addChild(monster);
	
	addblock("11");
	addblock("12");
	addblock("13");
	addblock("14");
	addblock("15");
	addblock("16");
	addblock("17");
	addblock("18");
	addblock("19");
}

void SecondScene::addblock(std::string blockname) {
	TMXObjectGroup *mon = monster->getObjectGroup(blockname);
	ValueVector moncontainer = mon->getObjects();

	blockname = blockname + ".png";
	for (auto obj : moncontainer) {
		ValueMap values = obj.asValueMap();

		int x = values.at("x").asInt();
		int y = values.at("y").asInt();

		Sprite *block = Sprite::create(blockname);
		block->setAnchorPoint(Vec2(0, 0));
		block->setPosition(x, y);
		block->setPhysicsBody(PhysicsBody::createBox(Size(block->getContentSize().width, block->getContentSize().height)));
		monmap.insert(i, block);
		block->setTag(i);
		block->getPhysicsBody()->getShape(0)->setRestitution(1.0f);
		block->getPhysicsBody()->getShape(0)->setFriction(0.0f);
		block->getPhysicsBody()->setContactTestBitmask(0xFFFFFFFF);
		block->getPhysicsBody()->setGravityEnable(false);
		i++;
		count++;
		monster->addChild(block);
	}
}

//菜单栏
void SecondScene::openmenu(Ref *ref) {
	auto restartitem = MenuItemFont::create("Restart", CC_CALLBACK_1(SecondScene::menurestart, this));
	auto quititem = MenuItemFont::create("Quit", CC_CALLBACK_1(SecondScene::menuquit, this));
	auto backitem = MenuItemFont::create("Back", CC_CALLBACK_1(SecondScene::menuback, this));
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

void SecondScene::menuback(Ref *ref) {
	auto startscene = HelloWorld::createScene();
	Director::getInstance()->replaceScene(startscene);
}

void SecondScene::menurestart(Ref *ref)
{
	auto SecondScene = Level_1Scene::createScence();
	Director::getInstance()->replaceScene(SecondScene);
}

void SecondScene::menuquit(Ref *ref) {
	Director::getInstance()->end();
}

int SecondScene::getScore() {
	return score;
}


//更新分数
void SecondScene::updateScore(float dt) {
	score -= dt;
	char *str = new char[10];
	sprintf(str, "%d", score);
	score_label->setString(str);
}

//判断是否结束游戏
void SecondScene::ifwin() {
	if (contact_count > 10) {
		this->unscheduleAllSelectors();
		Director::getInstance()->pause();
		auto win = Label::createWithTTF("You Score is :", "fonts/Marker Felt.ttf", 50);
		win->setColor(ccc3(0, 0, 0));
		win->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 15);
		addChild(win);

		auto win_score = Label::createWithTTF("", "fonts/Marker Felt.ttf", 50);
		char *str = new char[10];
		sprintf(str, "%d", score);
		win_score->setString(str);
		win_score->setColor(ccc3(0, 0, 0));
		win_score->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 30);
		addChild(win_score);

		auto item1 = MenuItemLabel::create(Label::createWithSystemFont("Next Level", "fonts/Marker Felt.ttf", 30), CC_CALLBACK_1(SecondScene::nextScene, this));
		auto menu = Menu::create(item1, NULL);
		menu->setPosition(Director::getInstance()->getVisibleSize().width / 2, Director::getInstance()->getVisibleSize().height / 2 + 40);
		menu->alignItemsVerticallyWithPadding(30);
		this->addChild(menu, 1);
	}
}


void SecondScene::nextScene(Ref *ref) {
	this->stopAllActions();
	auto scene = PushBoxScene::createScene();
	Director::getInstance()->replaceScene(scene);
}