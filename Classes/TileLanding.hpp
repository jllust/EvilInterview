//
//  TileLanding.hpp
//  Evil
//
//  Created by Jason on 1/26/16.
//
//

#ifndef TileLanding_hpp
#define TileLanding_hpp

#include "cocos2d.h"

USING_NS_CC;

class TileLanding : public Sprite
{
public:
    char correctLetter;
    
    static TileLanding* create();
    
    virtual void onEnter();
    
    const Vec2 glPosition();
};

#endif /* TileLanding_hpp */
