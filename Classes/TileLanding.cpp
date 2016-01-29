//
//  TileLanding.cpp
//  Evil
//
//  Created by Jason on 1/26/16.
//
//

#include "TileLanding.hpp"
#include "LetterTile.hpp"

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
        tile->goHome();
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
