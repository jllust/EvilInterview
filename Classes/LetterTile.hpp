//
//  LetterTile.hpp
//  Evil
//
//  Created by Jason on 1/26/16.
//
//

#ifndef LetterTile_hpp
#define LetterTile_hpp

#include "cocos2d.h"

USING_NS_CC;

class LetterTile : public Layer
{
public:
    char letter;
    Vec2 home;
    
    virtual bool init();
    
    CREATE_FUNC(LetterTile);
    
    void uiPosition(Vec2 &pos);
    
    virtual void onEnter();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void update(float delta);
};

#endif /* LetterTile_hpp */
