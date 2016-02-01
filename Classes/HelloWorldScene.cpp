#include "HelloWorldScene.h"
#include <algorithm>
#include <random>
#include <chrono>       // std::chrono::system_clock
#include <iostream>
#include "audio/include/SimpleAudioEngine.h"

#define DEBUG_WIN 0

Scene* HelloWorld::createScene() {
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init() {
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() ) return false;
    
#if DEBUG_WIN
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
#endif
    
    //std::srand(std::time(0));
    wordsPlayed = 0;
    gameWords = FileUtils::getInstance()->getValueMapFromFile("evilwords.plist")["words"].asValueVector();
    
    // get the screen sizes so assets can be centered
    ////
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a large back ground image as the games stage
    ////
    auto bg = Sprite::createWithSpriteFrameName("gamebg.png");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    addChild(bg, 0);
    
    userNeedsHelp1 = true;
    userNeedsHelp2 = true;
    
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
            CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/click0.mp3");
            userNeedsHelp1 = false;
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
    if (didWin == w_win) userSolvedPhrase(); //cout << "You win the Empire" << endl;
    else if (didWin == w_wrong) {
        helpTheUser2(0.f); //cout << "That's not the final word" << endl;
        userNeedsHelp2 = false;
    }

}

void HelloWorld::startIntro() {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    //Intro to get the user started
    ////
    auto intro = SkeletonAnimation::createWithFile("solveintro.json", "spine.atlas");
    intro->setPosition(540, 410);
    intro->setScale(0.5);
    intro->setAnimation(0, "intro", false);
    addChild(intro);

    helpAni = SkeletonAnimation::createWithFile("help0.json", "spine.atlas");
    helpAni->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
    helpAni->setAnimation(0, "help0", false);
    addChild(helpAni, 20);
    
    Vec2 pos;
    auto iter = letterTiles.begin();
    while(iter != letterTiles.end()) {
        pos = (*iter)->getPosition();
        pos.y = -80;
        (*iter)->setPosition(pos);
        ++iter;
    }
    dropTile(true, true);
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/mj_new_set_tiles.caf");
    scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::postIntroStart), 3);
}

void HelloWorld::postIntroStart(float d) {
    CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("sounds/rackrampup.mp3", false);
    Size visibleSize = Director::getInstance()->getVisibleSize();
    countDown = CountDown::create();
    countDown->delegate = this;
    countDown->setCascadeOpacityEnabled(true);
    countDown->setPosition(visibleSize.width-150, visibleSize.height-100);
    addChild(countDown);
    countDown->setOpacity(0);
    countDown->runAction(FadeIn::create(1));
    
    scheduleOnce(CC_SCHEDULE_SELECTOR(HelloWorld::helpTheUser1), 8);
}

//Once the user has sovled the phrase we will start some fanfair as a reward
////
void HelloWorld::userSolvedPhrase() {
    int i = 0;
    LetterTile* aTile;
    ScaleTo* s1;
    ScaleTo* s2;
    Sequence* sq;

#if (DEBUG_WIN == 1)
    auto iter = letterTiles.begin();
    while(iter != letterTiles.end()) {
        aTile = (*iter);
#else
    auto iter = landings.begin();
    while(iter != landings.end()) {
        aTile = (*iter)->tile;
#endif
        if(!(*iter)->isVisible()) {
            ++iter;
            continue;
        }
        aTile->getEventDispatcher()->removeEventListenersForTarget((*iter));
        s1 = ScaleTo::create(0.1, 1.2);
        s2 = ScaleTo::create(0.1, 1);
        sq = Sequence::create(DelayTime::create(0.1*i), s1, s2, DelayTime::create(1-(0.1*i)), NULL);
        aTile->runAction(RepeatForever::create(sq));
        ++i;
        ++iter;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto twirl = SkeletonAnimation::createWithFile("twirl.json", "spine.atlas");
    twirl->setPosition(visibleSize.width*0.5, visibleSize.height*0.5);
    twirl->setScale(0.5);
    twirl->setAnimation(0, "intro", false);
    addChild(twirl);
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/bingo.caf");
    countDown->stop();
    countDown->runAction(ScaleTo::create(0.25, 0));
}

//This method is going to keep the home tray clean and compressed
//it will resize to the remaining letters in the tray and allow reordering
////
void HelloWorld::dropTile(bool animated, bool delay) {
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
        pos = Vec2(lastX , 0);
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
            if (delay) {
                //If delayed this will be an animation from the bottom of the screen to start the scene
                ////
                auto moveHome = MoveTo::create(0.75, v);
                auto elast = EaseElasticOut::create(moveHome);
                tempSort[i]->runAction(Sequence::create(DelayTime::create((i*0.1)), elast, NULL));
            } else {
                auto moveHome = MoveTo::create(0.15, v);
                tempSort[i]->runAction(moveHome);
                
            }
        } else
            tempSort[i]->setPosition(v);
    }
}

void HelloWorld::onEnter() {
    Layer::onEnter();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    //Character for fun user feedback
    ////
    auto teaser = SkeletonAnimation::createWithFile("teaser.json", "spine.atlas");
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
    dropTile(false);
    
    Rect r;
    long lastX = 0;
    for (int j = 0; j < i; ++j) {
        aLanding = landings[j];
        r = aLanding->getBoundingBox();
        aLanding->setPosition(lastX + (r.size.width*0.5), 0);
        lastX += r.size.width;
    }
    landingTray->setPosition(Vec2((visibleSize.width*0.5)-(lastX*0.5), 300));
    
    startIntro();
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic("sounds/rackrampup.mp3");
}

void HelloWorld::update(float delta) {

}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event) {
    userSolvedPhrase();
}


void HelloWorld::helpTheUser1(float d) {
    if (userNeedsHelp1) helpAni->setAnimation(0, "help1", false);
    
}

void HelloWorld::helpTheUser2(float d) {
    if (userNeedsHelp2) helpAni->setAnimation(0, "help2", false);
}

void HelloWorld::outOfTime() {
    CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/wr_player_lose.caf");
}
