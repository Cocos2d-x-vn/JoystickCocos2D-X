#ifndef STAGE_SCENE_H
#define STAGE_SCENE_H

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "../SneakyInput/SneakyButton.h"
#include "../SneakyInput/SneakyButtonSkinnedBase.h"
#include "../SneakyInput/SneakyJoystick.h"
#include "../SneakyInput/SneakyJoystickSkinnedBase.h"
#include "../Entities/Player.h"
#include "../ContactListener.h"
#include "../Extras/ParallaxScrollNode.h"
#include "../Extras/ParallaxNodeExtras.h"

class StageScene: public cocos2d::CCLayer {

public:

	virtual bool init();
	static cocos2d::CCScene* scene();
	// a selector callback
	void menuCloseCallback(CCObject* pSender);
	void update(float dt);
	void setHUDLayer(CCLayer *layer);
	void setBackgroundLayer(ParallaxScrollNode *layer);
	static StageScene *createWithHUDAndBackGround(CCLayer *hudLayer,
			ParallaxScrollNode *backgroundLayer);

private:

	ContactListener *contactListener;
	Player *player;
	b2World *world;
	SneakyJoystick *leftJoystick;
	SneakyButton *action1Button;
	cocos2d::CCLayer *hudLayer;
	ParallaxScrollNode *backgroundLayer;
	cocos2d::CCSize worldSize;

	void updatePhysics(float dt);
	void createPhysics();
};

#endif
