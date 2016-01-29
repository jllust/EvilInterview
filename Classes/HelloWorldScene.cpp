#include "HelloWorldScene.h"
#include <algorithm>
#include <random>
#include <chrono>       // std::chrono::system_clock
#include <iostream>

Scene* HelloWorld::createScene() {
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) return false;
    
    //std::srand(std::time(0));
    wordsPlayed = 0;
    gameWords = FileUtils::getInstance()->getValueMapFromFile("evilwords.plist")["words"].asValueVector();
    
    // get the screen sizes so assets can be centered
    ////
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a label shows the game title
    ////
    auto label = Label::createWithTTF("Evil Words", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width/2,
                            visibleSize.height - label->getContentSize().height));
    addChild(label, 1);

    // add a large back ground image as the games stage
    ////
    auto bg = Sprite::createWithSpriteFrameName("gamebg.jpg");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    addChild(bg, 0);
    
    return true;
}

//This method will be test the users dragged tile is it going to be "played"
//or dropped aimlessly and will go back to the home tray
////
bool HelloWorld::testDrop(LetterTile* letter) {
    auto iter = landings.begin();
    while(iter != landings.end()) {
        Vec2 t = letter->uiPosition();
        t -= landingTray->getPosition();
        Rect r = (*iter)->getBoundingBox();
        if ((*iter)->tile == letter) (*iter)->tile = nullptr;
        if ( r.containsPoint(t) && (*iter)->isVisible() ) {
            (*iter)->placeTile(letter);
            Vec2 n = (*iter)->getPosition();
            n += landingTray->getPosition();
            letter->uiPosition(n);
            (*iter)->tile = letter;
            testWord();
            return true;
        }
        ++iter;
    }
    return false;
}

//The method to test if all the tiles have been used
// then wether the phrase is correct or just a jumble
////
void HelloWorld::testWord() {
    typedef enum : int {
        w_wrong,
        w_win,
        w_noncomplete,
    } winstate;
    
    winstate didWin = w_win;
    
    //First we will check if all tile landings have letters in them.
    ////
    auto iter = landings.begin();
    while(iter != landings.end()) {
        if ((*iter)->isVisible() && (*iter)->tile == nullptr) {
            cout << "Not finished yet" << endl;
            return;
        }
        ++iter;
    }
    
    //Then we will check if they have the winning letters in them.
    ////
    iter = landings.begin();
    while(iter != landings.end()) {
        if ((*iter)->isVisible()) {
            if ((*iter)->tile != nullptr) {
                if ((*iter)->correctLetter != (*iter)->tile->letter) {
                    didWin = w_wrong;
                    break;
                }
            } else {
                didWin = w_noncomplete;
                break;
            }
        }
        ++iter;
    }
    if (didWin == w_win) cout << "You win the Empire" << endl;
    else if (didWin == w_wrong) cout << "That's not the final word" << endl;

    //TODO:turn off the tile touches.
}

//This method is going to keep the home tray clean and compressed
//it will resize to the remaining letters in the tray and allow reordering
////
void HelloWorld::dropTile(LetterTile* tile, bool animated) {
    vector<LetterTile*> tempSort;
    vector<Vec2> tempPos;
    auto iter = letterTiles.begin();
    while(iter != letterTiles.end()) {
        if ((*iter)->landing == nullptr) tempSort.push_back((*iter));
        ++iter;
    }
    sort(tempSort.begin(), tempSort.end(), [](LetterTile* a, LetterTile* b) {
        return a->getPosition().x < b->getPosition().x;
    });
    
    Rect r = letterTiles[0]->getBoundingBox();
    float lastX = 0;
    Vec2 pos;
    iter = tempSort.begin();
    while(iter != tempSort.end()) {
        pos = Vec2(lastX + (r.size.width*0.5), 0);
        lastX += r.size.width + 5;
        tempPos.push_back(pos);
        ++iter;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    for (int i = 0; i < tempSort.size(); ++i) {
        Vec2 v = tempPos[i];
        v += Vec2((visibleSize.width*0.5)-(lastX*0.5), 75);
        if (animated) {
            tempSort[i]->stopAllActions();
            MoveTo* moveHome = MoveTo::create(0.15, v);
            tempSort[i]->runAction(moveHome);
        } else
            tempSort[i]->setPosition(v);
    }

}

void HelloWorld::onEnter() {
    Layer::onEnter();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //Character for fun user feedback
    ////
    auto teaser = SkeletonAnimation::createWithFile("teaser.json", "teaser.atlas");
    teaser->setPosition(150, 350);
    teaser->setScale(0.7);
    teaser->setAnimation(0, "look", true);
    teaser->setAnimation(1, "tail", true);
    teaser->setAnimation(2, "blink", true);
    teaser->setAnimation(3, "scarf", true);
    addChild(teaser);
    
    landingTray = Node::create();
    addChild(landingTray);
    
    currentWordIdx = arc4random() % gameWords.size();
    std::string str = gameWords[currentWordIdx].asString();
    std::transform(str.begin(), str.end(),str.begin(), ::toupper);
    const char* aWord = str.c_str();

    LetterTile* aTile;
    TileLanding* aLanding;
    int i = 0;
    while (aWord[i] != 0) {
        if (aWord[i] != ' ') {
            aTile = LetterTile::create();
            aTile->delegate = this;
            aTile->letter = aWord[i];
            letterTiles.push_back(aTile);
            addChild(aTile);
        }
        aLanding = TileLanding::create();
        aLanding->correctLetter = aWord[i];
        landings.push_back(aLanding);
        landingTray->addChild(aLanding);
        ++i;
    }
    random_shuffle ( letterTiles.begin(), letterTiles.end() , [](int d){return arc4random()%d;});
    dropTile(nullptr, false);
    
    Rect r;
    long lastX = 0;
    for (int j = 0; j < i; ++j) {
        aLanding = landings[j];
        r = aLanding->getBoundingBox();
        aLanding->setPosition(lastX + (r.size.width*0.5), 0);
        lastX += r.size.width;
    }
    landingTray->setPosition(Vec2((visibleSize.width*0.5)-(lastX*0.5), 300));
}

void HelloWorld::update(float delta) {

}
