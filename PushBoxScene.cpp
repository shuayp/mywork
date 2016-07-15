#include "PushBoxScene.h"
#include "cocos2d.h"
#define database UserDefault::getInstance()

const float SIZE_WSAD = 100.0;
const int FONT_SIZE = 100;


PushBoxScene::PushBoxScene()
{

}

PushBoxScene::~PushBoxScene()
{

}


Scene* PushBoxScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PushBoxScene::create();
	scene->addChild(layer);
	return scene;
}



bool PushBoxScene::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	initTouchEvent();

	TMXTiledMap* map = TMXTiledMap::create("PushBox/mymap.tmx");
	map->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	map->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(map, 0);

	cocos2d::Map<std::string, Sprite*> check;

	TMXObjectGroup* wall_objects = map->getObjectGroup("wall");
	ValueVector wall_container = wall_objects->getObjects();

	for (auto obj : wall_container){
		ValueMap values = obj.asValueMap();

		int x = values.at("x").asInt();
		int y = values.at("y").asInt();

		Sprite* wall = Sprite::create("PushBox/wall.png");
		wall->setAnchorPoint(Vec2(0, 0));
		wall->setPosition(x, y + 64);
		wall_vector.pushBack(wall);
		map->addChild(wall);
	}

	TMXObjectGroup* destination_objects = map->getObjectGroup("goal");
	ValueVector destination_container = destination_objects->getObjects();

	auto obj0 = destination_container;

	ValueMap values1 = obj0.at(0).asValueMap();

	int x1 = values1.at("x").asInt();
	int y1 = values1.at("y").asInt();

	destination1 = Sprite::create("PushBox/goal.png");
	destination1->setAnchorPoint(Vec2(0, 0));
	destination1_position = Vec2(x1, y1 + 64);
	destination1->setPosition(destination1_position);
	map->addChild(destination1);

	destination3 = Sprite::create("PushBox/finish.png");
	destination3->setVisible(false);
	destination3->setAnchorPoint(Vec2(0, 0));
	destination3->setPosition(destination1_position);
	map->addChild(destination3);


	ValueMap values2 = obj0.at(1).asValueMap();

	int x2 = values2.at("x").asInt();
	int y2 = values2.at("y").asInt();

	destination2 = Sprite::create("PushBox/goal.png");
	destination2->setAnchorPoint(Vec2(0, 0));
	destination2_position = Vec2(x2, y2 + 64);
	destination2->setPosition(destination2_position);
	map->addChild(destination2);

	destination4 = Sprite::create("PushBox/finish.png");
	destination4->setVisible(false);
	destination4->setAnchorPoint(Vec2(0, 0));
	destination4->setPosition(destination2_position);
	map->addChild(destination4);



	TMXObjectGroup* box_objects = map->getObjectGroup("box");
	ValueVector box_container = box_objects->getObjects();

	auto obj1 = box_container;

	ValueMap values3 = obj1.at(0).asValueMap();

	int x3 = values3.at("x").asInt();
	int y3 = values3.at("y").asInt();

	box1 = Sprite::create("PushBox/box.png");
	box1->setAnchorPoint(Vec2(0, 0));
	box1_position = Vec2(x3, y3 + 64);
	box1->setPosition(box1_position);
	map->addChild(box1);


	ValueMap values4 = obj1.at(1).asValueMap();

	int x4 = values4.at("x").asInt();
	int y4 = values4.at("y").asInt();

	box2 = Sprite::create("PushBox/box.png");
	box2->setAnchorPoint(Vec2(0, 0));
	box2_position = Vec2(x4, y4 + 64);
	box2->setPosition(box2_position);
	map->addChild(box2);
	
	TMXObjectGroup* player_objects = map->getObjectGroup("player");
	ValueVector player_container = player_objects->getObjects();

	for (auto obj : player_container){
		ValueMap values = obj.asValueMap();

		int x = values.at("x").asInt();
		int y = values.at("y").asInt();

		player = Sprite::create("PushBox/player.png");
		player->setAnchorPoint(Vec2(0, 0));
		player->setPosition(x, y + 64);
		player_position = Vec2(x, y + 64);
		map->addChild(player);
	}
	return true;
}



void PushBoxScene::initTouchEvent(){
	auto menu = Menu::create();
	menu->setPosition(visibleSize.width,0);
	menu->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	this->addChild(menu,10);

	auto label_W = Label::createWithTTF("W", "fonts/Marker Felt.ttf", FONT_SIZE);
	auto label_S = Label::createWithTTF("S", "fonts/Marker Felt.ttf", FONT_SIZE);
	auto label_A = Label::createWithTTF("A", "fonts/Marker Felt.ttf", FONT_SIZE);
	auto label_D = Label::createWithTTF("D", "fonts/Marker Felt.ttf", FONT_SIZE);

	auto button_up = MenuItemLabel::create(label_W, CC_CALLBACK_1(PushBoxScene::onUpPressed, this));
	auto button_down = MenuItemLabel::create(label_S, CC_CALLBACK_1(PushBoxScene::onDownPressed, this));
	auto button_left = MenuItemLabel::create(label_A, CC_CALLBACK_1(PushBoxScene::onLeftPressed, this));
	auto button_right = MenuItemLabel::create(label_D, CC_CALLBACK_1(PushBoxScene::onRightPressed, this));
	
	button_up->setPosition(SIZE_WSAD * -1, SIZE_WSAD * 2);
	button_down->setPosition(SIZE_WSAD * -1, SIZE_WSAD * 0);
	button_left->setPosition(SIZE_WSAD * -2, SIZE_WSAD * 1);
	button_right->setPosition(SIZE_WSAD * 0, SIZE_WSAD * 1);

	button_up->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	button_down->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	button_left->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
	button_right->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

	menu->addChild(button_up);
	menu->addChild(button_down);
	menu->addChild(button_left);
	menu->addChild(button_right);
}







void PushBoxScene::onRightPressed(Ref* sender)
{
	if (!if_win(this))
	{

		Vec2 right = Vec2(player_position.x + 64, player_position.y);
		Vec2 rightright = Vec2(player_position.x + 128, player_position.y);

		bool wall_on_right = 0;
		bool wall_on_rightright = 0;
		bool box1_on_right = 0;
		bool box1_on_rightright = 0;
		bool box2_on_right = 0;
		bool box2_on_rightright = 0;

		int vector_size = wall_vector.size();

		//judge if a wall on the right
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == right) {
				wall_on_right = 1;
				break;
			}
		}
		//judge if a wall on the rightright
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == rightright) {
				wall_on_rightright = 1;
				break;
			}
		}
		//judge box1
		if (box1_position == right) {
			box1_on_right = 1;
		}
		if (box1_position == rightright) {
			box1_on_rightright = 1;
		}
		//judge box2
		if (box2_position == right) {
			box2_on_right = 1;
		}
		if (box2_position == rightright) {
			box2_on_rightright = 1;
		}

		//conditions
		if (wall_on_right) {

		}
		else {
			if (box1_on_right && wall_on_rightright || box2_on_right && wall_on_rightright) {

			}
			else if (box1_on_right && box2_on_rightright || box2_on_right && box1_on_rightright) {

			}
			else {
				if (box1_on_right) {
					player_position = right;
					player->setPosition(player_position);
					box1_position = rightright;
					box1->setPosition(box1_position);
					step_count++;
				}
				else if (box2_on_right) {
					player_position = right;
					player->setPosition(player_position);
					box2_position = rightright;
					box2->setPosition(box2_position);
					step_count++;
				}
				else {
					player_position = right;
					player->setPosition(player_position);
					step_count++;
				}

				if (if_win(this)) {
					show_win(this);
				}
			}
		}
	}
}

void PushBoxScene::onLeftPressed(Ref* sender)
{
	if (!if_win(this))
	{
		/*player_position = Vec2(player_position.x - 64, player_position.y);
		player->setPosition(player_position);

		step_count++;*/

		Vec2 left = Vec2(player_position.x - 64, player_position.y);
		Vec2 leftleft = Vec2(player_position.x - 128, player_position.y);

		bool wall_on_left = 0;
		bool wall_on_leftleft = 0;
		bool box1_on_left = 0;
		bool box1_on_leftleft = 0;
		bool box2_on_left = 0;
		bool box2_on_leftleft = 0;

		int vector_size = wall_vector.size();

		//judge if a wall on the left
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == left) {
				wall_on_left = 1;
				break;
			}
		}
		//judge if a wall on the leftleft
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == leftleft) {
				wall_on_leftleft = 1;
				break;
			}
		}
		//judge box1
		if (box1_position == left) {
			box1_on_left = 1;
		}
		if (box1_position == leftleft) {
			box1_on_leftleft = 1;
		}
		//judge box2
		if (box2_position == left) {
			box2_on_left = 1;
		}
		if (box2_position == leftleft) {
			box2_on_leftleft = 1;
		}

		//conditions
		if (wall_on_left) {

		}
		else {
			if (box1_on_left && wall_on_leftleft || box2_on_left && wall_on_leftleft) {

			}
			else if (box1_on_left && box2_on_leftleft || box2_on_left && box1_on_leftleft) {

			}
			else {
				if (box1_on_left) {
					player_position = left;
					player->setPosition(player_position);
					box1_position = leftleft;
					box1->setPosition(box1_position);
					step_count++;
				}
				else if (box2_on_left) {
					player_position = left;
					player->setPosition(player_position);
					box2_position = leftleft;
					box2->setPosition(box2_position);
					step_count++;
				}
				else {
					player_position = left;
					player->setPosition(player_position);
					step_count++;
				}

				if (if_win(this)) {
					show_win(this);
				}
			}
		}
	}
}

void PushBoxScene::onUpPressed(Ref* sender)
{
	if (!if_win(this))
	{
		/*player_position = Vec2(player_position.x, player_position.y + 64);
		player->setPosition(player_position);

		step_count++;*/

		Vec2 up = Vec2(player_position.x, player_position.y + 64);
		Vec2 upup = Vec2(player_position.x, player_position.y + 128);

		bool wall_on_up = 0;
		bool wall_on_upup = 0;
		bool box1_on_up = 0;
		bool box1_on_upup = 0;
		bool box2_on_up = 0;
		bool box2_on_upup = 0;

		int vector_size = wall_vector.size();

		//judge if a wall on the up
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == up) {
				wall_on_up = 1;
				break;
			}
		}
		//judge if a wall on the upup
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == upup) {
				wall_on_upup = 1;
				break;
			}
		}
		//judge box1
		if (box1_position == up) {
			box1_on_up = 1;
		}
		if (box1_position == upup) {
			box1_on_upup = 1;
		}
		//judge box2
		if (box2_position == up) {
			box2_on_up = 1;
		}
		if (box2_position == upup) {
			box2_on_upup = 1;
		}

		//conditions
		if (wall_on_up) {

		}
		else {
			if (box1_on_up && wall_on_upup || box2_on_up && wall_on_upup) {

			}
			else if (box1_on_up && box2_on_upup || box2_on_up && box1_on_upup) {

			}
			else {
				if (box1_on_up) {
					player_position = up;
					player->setPosition(player_position);
					box1_position = upup;
					box1->setPosition(box1_position);
					step_count++;
				}
				else if (box2_on_up) {
					player_position = up;
					player->setPosition(player_position);
					box2_position = upup;
					box2->setPosition(box2_position);
					step_count++;
				}
				else {
					player_position = up;
					player->setPosition(player_position);
					step_count++;
				}

				if (if_win(this)) {
					show_win(this);
				}
			}
		}
	}
}

void PushBoxScene::onDownPressed(Ref* sender)
{
	/*player_position = Vec2(player_position.x, player_position.y - 64);
	player->setPosition(player_position);

	step_count++;*/
	if (!if_win(this)) {


		Vec2 down = Vec2(player_position.x, player_position.y - 64);
		Vec2 downdown = Vec2(player_position.x, player_position.y - 128);

		bool wall_on_down = 0;
		bool wall_on_downdown = 0;
		bool box1_on_down = 0;
		bool box1_on_downdown = 0;
		bool box2_on_down = 0;
		bool box2_on_downdown = 0;

		int vector_size = wall_vector.size();

		//judge if a wall on the down
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == down) {
				wall_on_down = 1;
				break;
			}
		}
		//judge if a wall on the downdown
		for (int i = 0; i < vector_size; i++) {
			if (wall_vector.at(i)->getPosition() == downdown) {
				wall_on_downdown = 1;
				break;
			}
		}
		//judge box1
		if (box1_position == down) {
			box1_on_down = 1;
		}
		if (box1_position == downdown) {
			box1_on_downdown = 1;
		}
		//judge box2
		if (box2_position == down) {
			box2_on_down = 1;
		}
		if (box2_position == downdown) {
			box2_on_downdown = 1;
		}

		//conditions
		if (wall_on_down) {

		}
		else {
			if (box1_on_down && wall_on_downdown || box2_on_down && wall_on_downdown) {

			}
			else if (box1_on_down && box2_on_downdown || box2_on_down && box1_on_downdown) {

			}
			else {
				if (box1_on_down) {
					player_position = down;
					player->setPosition(player_position);
					box1_position = downdown;
					box1->setPosition(box1_position);
					step_count++;
				}
				else if (box2_on_down) {
					player_position = down;
					player->setPosition(player_position);
					box2_position = downdown;
					box2->setPosition(box2_position);
					step_count++;
				}
				else {
					player_position = down;
					player->setPosition(player_position);
					step_count++;
				}

				if (if_win(this)) {
					show_win(this);
				}
			}
		}

	}
}



bool PushBoxScene::if_win(Ref* sender)
{
	if (box1_position == destination1_position){
		box1->setVisible(false);
		destination3->setVisible(true);
		destination1->setVisible(false);
	}
	else if (box2_position == destination1_position) {
		box2->setVisible(false);
		destination3->setVisible(true);
		destination1->setVisible(false);
	}
	else if (box2_position == destination2_position){
		box2->setVisible(false);
		destination4->setVisible(true);
		destination2->setVisible(false);
	}
	else if (box1_position == destination2_position) {
		box1->setVisible(false);
		destination4->setVisible(true);
		destination2->setVisible(false);
	}
	else {
		box2->setVisible(true);
		box1->setVisible(true);
		destination2->setVisible(true);
		destination4->setVisible(false);
		destination1->setVisible(true);
		destination3->setVisible(false);
	}
	if (box1_position == destination1_position && box2_position == destination2_position
		|| box1_position == destination2_position && box2_position == destination1_position) {
		least_step_count = step_count;
		destination1->setVisible(false);
		destination2->setVisible(false);
		destination3->setVisible(true);
		destination4->setVisible(true);
		box1->setVisible(false);
		box2->setVisible(false);
		//detect the XML file
		if (!database->getBoolForKey("isExist")){
			database->setBoolForKey("isExist", true);
		}
		//save the least steps
		database->setIntegerForKey("least steps", least_step_count);

		return true;
	}
	else {
		return false;
	}
}


void PushBoxScene::show_win(Ref* sender)
{
	auto menu_win = Menu::create();
	menu_win->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	
	char least_steps[30];
	sprintf(least_steps, "Win !\n Your steps:%d", least_step_count);
	label_win = Label::createWithTTF("", "fonts/Marker Felt.ttf", FONT_SIZE);
	label_win->setString(least_steps);
	auto menuitemlabel = MenuItemLabel::create(label_win);
	menu_win->addChild(menuitemlabel);

	this->addChild(menu_win, 11);
	
}