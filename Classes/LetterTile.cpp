//
//  LetterTile.cpp
//  Evil
//
//  Created by Jason on 1/26/16.
//
//

#include "LetterTile.hpp"
#include "HelloWorldScene.h"

using namespace std;

// on "init" you need to initialize your instance
bool LetterTile::init() {
    if ( !Layer::init() ) return false;
    
    // Setup the touch system so tiles can be interacted with
    
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = CC_CALLBACK_2(LetterTile::onTouchBegan, this);
    touchListener->onTouchMoved = CC_CALLBACK_2(LetterTile::onTouchMoved, this);
    touchListener->onTouchEnded = CC_CALLBACK_2(LetterTile::onTouchEnded, this);
    touchListener->onTouchCancelled = CC_CALLBACK_2(LetterTile::onTouchCancelled, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    return true;
}

void LetterTile::uiPosition(Vec2 &pos) {
    pos -= _parent->getPosition();
    setPosition(pos);
}

const Vec2 LetterTile::uiPosition() {
    Vec2 v = _position;
    v += _parent->getPosition();
    return v;
}

void LetterTile::onEnter() {
    Node::onEnter();
    
    // start to add assets once this layer is added to the draw heirachy
    auto tile = Sprite::createWithSpriteFrameName("tile.png");
    //tile->setAnchorPoint(Vec2());
    addChild(tile);
    
    Rect r = tile->getBoundingBox();
    setContentSize(r.size);
    auto label = Label::createWithTTF(string(&letter), "fonts/Marker Felt.ttf", 42);
    //label->setPosition(r.size.width*0.5, r.size.height*0.5);
    addChild(label);
}

bool LetterTile::onTouchBegan(Touch *touch, Event *event) {
    Vec2 t = touch->getLocation() - _parent->getPosition();
    Rect r = getBoundingBox();
    r.origin -= _contentSize.width*0.5;
    if (r.containsPoint(t)) {
        if (landing) {
            landing->tile = nullptr;
            landing = nullptr;
        }
        setZOrder(10);
        setPosition(t);
        return true;
    }
    return false;
}

void LetterTile::onTouchMoved(Touch *touch, Event *event) {
    Vec2 t = touch->getLocation() - _parent->getPosition();
    setPosition(t);
}

void LetterTile::onTouchEnded(Touch *touch, Event *event) {
    setZOrder(0);
    if (!delegate) return;
    if ( delegate->testDrop(this) ) {
      
    }
    delegate->dropTile(this);
}

void LetterTile::update(float delta) {
    
}