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

class LetterTile;

class TileLanding : public Sprite
{
public:
    char correctLetter;
    LetterTile* tile;
    
    static TileLanding* create();
    
    virtual void onEnter();
    
    void placeTile(LetterTile* newtile);
    const Vec2 glPosition();
};

#endif /* TileLanding_hpp */
