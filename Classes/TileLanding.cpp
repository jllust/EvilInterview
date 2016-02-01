//
//  TileLanding.cpp
//  Evil
//
//  Created by Jason on 1/26/16.
//
//

#include "TileLanding.hpp"
#include "LetterTile.hpp"

using namespace std;

TileLanding* TileLanding::create() {
    TileLanding *pRet = new(std::nothrow) TileLanding(); \
    if (pRet && pRet->initWithSpriteFrameName("tray.png")) {
        pRet->tile = nullptr;
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

void TileLanding::onEnter() {
    Node::onEnter();
    if (correctLetter == ' ') {
        setVisible(false);
        _contentSize.width *= 0.5;
    }
}

void TileLanding::placeTile(LetterTile* newtile) {
    if (tile != nullptr) {
        tile->landing = nullptr;
        tile = nullptr;
    }
    if (newtile != nullptr) {
        tile = newtile;
        tile->landing = this;
    }
    
}

const Vec2 TileLanding::glPosition() {
    Vec2 v = _position;
    v -= getParent()->getPosition();
    return v;
}

void TileLanding::reveal() {
    Rect r = getBoundingBox();
    auto label = Label::createWithTTF(string(&correctLetter), "fonts/Marker Felt.ttf", 42);
    label->setPosition(r.size.width*0.5, r.size.height*0.5);
    label->setOpacity(0);
    addChild(label);
    label->runAction( Sequence::create(DelayTime::create(1), FadeIn::create(2), NULL) );
}

