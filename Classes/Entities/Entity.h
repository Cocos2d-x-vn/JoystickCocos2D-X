#ifndef ENTITY_H
#define ENTITY_H

#include "cocos2d.h"
#include "Box2D/Box2D.h"

#define PTM_RATIO 32.0

class Entity
{

public:

    static const int TAG_PLAYER = 1;
    static const int TAG_GROUND = 2;
    static const int TAG_WALL   = 3;

    Entity();
    virtual ~Entity();
    void setBatchNode(cocos2d::CCSpriteBatchNode *batchNode);
    cocos2d::CCSpriteBatchNode *getBatchNode();
    cocos2d::CCSprite *getSprite();
    int getDirection();
    int getState();
    virtual int getTag();

protected:

    cocos2d::CCSprite *sprite;
    cocos2d::CCRepeatForever *actionStateDefault;
    cocos2d::CCRepeatForever *actionStateMoving;
    cocos2d::CCAnimate *animateDefault;
    cocos2d::CCAnimate *animateMoving;
    cocos2d::CCAnimation *animationDefault;
    cocos2d::CCAnimation *animationMoving;
    cocos2d::CCSpriteBatchNode *batchNode;

    int direction;
    b2Body *body;
    b2World *world;
    int state; // 1 = default, 2 = moving, 3 = jumping

};

#endif // ENTITY_H
