#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "LetterTile.hpp"

USING_NS_CC;
using namespace std;

class HelloWorld : public Layer
{
public:
    static Scene* createScene();
    
    ValueVector gameWords;
    int wordsPlayed;
    int currentWordIdx;
    vector<LetterTile*> letterTiles;
    
    Layer* homeTray;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    virtual void onEnter();
    virtual bool onTouchBegan(Touch *touch, Event *event);
    virtual void onTouchMoved(Touch *touch, Event *event);
    virtual void onTouchEnded(Touch *touch, Event *event);
    virtual void update(float delta);
};

#endif // __HELLOWORLD_SCENE_H__
