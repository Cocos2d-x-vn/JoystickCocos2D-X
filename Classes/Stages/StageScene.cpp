#include "StageScene.h"

USING_NS_CC;

CCScene* StageScene::scene() {
	CCScene *scene = CCScene::create();

	CCLayer *hudLayer = CCLayer::create();
	scene->addChild(hudLayer, 2);

	ParallaxScrollNode *backgroundLayer = ParallaxScrollNode::create();
	scene->addChild(backgroundLayer, 0);

	StageScene *layer = StageScene::createWithHUDAndBackGround(hudLayer,
			backgroundLayer);
	scene->addChild(layer, 1);

	return scene;
}

StageScene *StageScene::createWithHUDAndBackGround(CCLayer *hudLayer,
		ParallaxScrollNode *backgroundLayer) {
	StageScene *object = new StageScene();
	object->setHUDLayer(hudLayer);
	object->setBackgroundLayer(backgroundLayer);

	if (object && object->init()) {
		object->autorelease();
		return object;
	} else {
		delete object;
		object = NULL;
		return NULL;
	}
}

bool StageScene::init() {
	//////////////////////////////
	// 1. super init first
	if (!CCLayer::init()) {
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png",
			"CloseSelected.png", this,
			menu_selector(StageScene::menuCloseCallback));

	pCloseItem->setPosition(
			ccp(
					origin.x + visibleSize.width
							- pCloseItem->getContentSize().width / 2,
					origin.y + visibleSize.height
							- pCloseItem->getContentSize().height / 2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	this->addChild(pMenu, 1);

	// create world/stage size
	worldSize = CCSize(1000, visibleSize.height);

	// create bottom label
	CCLabelTTF* label = CCLabelTTF::create("Joystick Cocos2D-X Version 2.2.2",
			"Arial", 12);

	label->setPosition(
			CCPoint(origin.x + visibleSize.width / 2,
					origin.y + label->getContentSize().height));

	hudLayer->addChild(label, 1);

	// add parallax background
	CCSprite *backgroundSprite1 = CCSprite::create("BackgroundGame.png");
	CCSprite *backgroundSprite2 = CCSprite::create("BackgroundGame.png");
	backgroundLayer->addInfiniteScrollXWithZ(0, CCPoint(1.0f, 1.0f),
			CCPoint(0, 0), backgroundSprite1, backgroundSprite2, NULL);

	// add joystick
	int joystickOffset = 10;

	CCRect joystickBaseDimensions = CCRect(0, 0, 64.0f, 64.0f);
	CCPoint joystickBasePosition = CCPoint(
			origin.x + (joystickBaseDimensions.size.width / 2) + joystickOffset,
			origin.y + (joystickBaseDimensions.size.height / 2)
					+ joystickOffset);

	CCRect accelButtonDimensions = CCRect(0, 0, 64.0f, 64.0f);
	CCPoint accelButtonPosition = CCPoint(
			origin.x + visibleSize.width
					- (joystickBaseDimensions.size.width / 2) - joystickOffset,
			origin.y + (joystickBaseDimensions.size.height / 2)
					+ joystickOffset);

	// add joystick base
	SneakyJoystickSkinnedBase *joystickBase =
			SneakyJoystickSkinnedBase::create();
	SneakyJoystick *joystick = new SneakyJoystick();
	joystick->initWithRect(joystickBaseDimensions);
	joystickBase->setBackgroundSprite(
			CCSprite::create("joystick/dpadDown.png"));
	joystickBase->setThumbSprite(CCSprite::create("joystick/joystickDown.png"));
	joystickBase->setJoystick(joystick);
	joystickBase->setPosition(joystickBasePosition);
	leftJoystick = joystickBase->getJoystick();
	hudLayer->addChild(joystickBase);

	// add joystick button
	SneakyButtonSkinnedBase *action1ButtonBase =
			SneakyButtonSkinnedBase::create();
	SneakyButton *button1 = new SneakyButton();
	button1->initWithRect(accelButtonDimensions);
	action1ButtonBase->setDefaultSprite(
			CCSprite::create("joystick/action1Down.png"));
	action1ButtonBase->setActivatedSprite(
			CCSprite::create("joystick/action1Down.png"));
	action1ButtonBase->setPressSprite(
			CCSprite::create("joystick/action1Up.png"));
	action1ButtonBase->setButton(button1);
	action1ButtonBase->setPosition(accelButtonPosition);
	action1Button = action1ButtonBase->getButton();
	hudLayer->addChild(action1ButtonBase);

	// create physics
	createPhysics();

	// add the player
	player = new Player();
	player->init(this, world);
	addChild(player->getBatchNode());

	// camera follow the player
	runAction(
			CCFollow::create(player->getSprite(),
					CCRect(0, 0, backgroundSprite1->getContentSize().width,
							backgroundSprite1->getContentSize().height)));

	// rest of framework init process
	scheduleUpdate();

	return true;
}

void StageScene::menuCloseCallback(CCObject* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
	CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
#endif
}

void StageScene::update(float dt) {
	// update the physics
	updatePhysics(dt);

	// update the player
	if (player) {
		player->update(dt);
		player->updateVelocity(leftJoystick->getVelocity());

		if (action1Button->getIsActive()) {
			player->actionButtonPressed(1);
		}
	}

	// update collisions
	std::vector<ContactData>::iterator pos;
	for (pos = contactListener->_contacts.begin();
			pos != contactListener->_contacts.end(); ++pos) {
		ContactData contact = *pos;

		// get the box2d bodies for each object
		b2Body *bodyA = contact.fixtureA->GetBody();
		b2Body *bodyB = contact.fixtureB->GetBody();

		if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
			Entity *entityA = (Entity*) bodyA->GetUserData();
			Entity *entityB = (Entity*) bodyB->GetUserData();

			int iTagA = entityA->getTag();
			int iTagB = entityB->getTag();

			if (iTagA == Entity::TAG_PLAYER) {

			} else if (iTagB == Entity::TAG_PLAYER) {

			}
		}
	}

	// update the background parallax
	//backgroundLayer->updateWithVelocity(Point(-2.0f, 0), dt);
	//backgroundLayer->updateWithXPosition(-player->getSprite()->getPosition().x, dt);
}

void StageScene::updatePhysics(float dt) {
	world->Step(dt, 10, 10);
}

void StageScene::createPhysics() {
	// world
	b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
	world = new b2World(gravity);

	// contact listener
	contactListener = new ContactListener();
	world->SetContactListener(contactListener);

	// create collision wall and ground
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, (66 / PTM_RATIO));
	b2Body *_groundBody;
	_groundBody = world->CreateBody(&groundBodyDef);

	b2EdgeShape groundBox;
	b2FixtureDef groundBoxDef;
	groundBoxDef.shape = &groundBox;

	// b2Fixture *_bottomFixture;

	groundBox.Set(b2Vec2(0, 0), b2Vec2(worldSize.width / PTM_RATIO, 0));
	//_bottomFixture = _groundBody->CreateFixture(&groundBoxDef);

	groundBox.Set(b2Vec2(0, 0), b2Vec2(0, worldSize.height / PTM_RATIO));
	_groundBody->CreateFixture(&groundBoxDef);

	groundBox.Set(b2Vec2(0, worldSize.height / PTM_RATIO),
			b2Vec2(worldSize.width / PTM_RATIO, worldSize.height / PTM_RATIO));
	_groundBody->CreateFixture(&groundBoxDef);

	groundBox.Set(
			b2Vec2(worldSize.width / PTM_RATIO, worldSize.height / PTM_RATIO),
			b2Vec2(worldSize.width / PTM_RATIO, 0));
	_groundBody->CreateFixture(&groundBoxDef);
}

void StageScene::setHUDLayer(CCLayer *layer) {
	hudLayer = layer;
}

void StageScene::setBackgroundLayer(ParallaxScrollNode *layer) {
	backgroundLayer = layer;
}
