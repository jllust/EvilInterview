#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "LetterTile.hpp"
#include "TileLanding.hpp"
#include <spine/spine-cocos2dx.h>

USING_NS_CC;
using namespace spine;
using namespace std;

class HelloWorld : public Layer
{
public:
    static Scene* createScene();
    
    ValueVector gameWords;
    int wordsPlayed;
    int currentWordIdx;
    vector<TileLanding*> landings;
    vector<LetterTile*> letterTiles;
    
    Node* homeTray;
    Node* landingTray;

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    bool testDrop(LetterTile* letter);
    void testWord();
    void dropTile(LetterTile* tile, bool animated = true);
    
    virtual void onEnter();
    virtual void update(float delta);
};

#endif // __HELLOWORLD_SCENE_H__
