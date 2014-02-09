#include "Entity.h"

USING_NS_CC;

Entity::Entity() {
}

Entity::~Entity() {
}

void Entity::setBatchNode(CCSpriteBatchNode *batchNode) {
	this->batchNode = batchNode;
}

CCSpriteBatchNode *Entity::getBatchNode() {
	return batchNode;
}

cocos2d::CCSprite *Entity::getSprite() {
	return sprite;
}

int Entity::getDirection() {
	return direction;
}

int Entity::getState() {
	return state;
}

int Entity::getTag() {
	return 0;
}
