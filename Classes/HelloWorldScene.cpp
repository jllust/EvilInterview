#include "HelloWorldScene.h"
#include <algorithm>
#include <random>
#include <chrono>       // std::chrono::system_clock

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
    
    std::srand(std::time(0));
    wordsPlayed = 0;
    gameWords = FileUtils::getInstance()->getValueMapFromFile("evilwords.plist")["words"].asValueVector();
    
//    auto touchListener = EventListenerTouchOneByOne::create();
//    touchListener->setSwallowTouches(true);
//    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
//    touchListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
//    touchListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
//    touchListener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
//    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
    
    // get the screen sizes so assets can be centered
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // add a label shows the game title
   
    auto label = Label::createWithTTF("Evil Words", "fonts/Marker Felt.ttf", 24);
    label->setPosition(Vec2(visibleSize.width/2,
                            visibleSize.height - label->getContentSize().height));
    addChild(label, 1);

    // add a large back ground image as the games stage

    auto bg = Sprite::createWithSpriteFrameName("gamebg.jpg");
    bg->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    addChild(bg, 0);
    
    return true;
}

bool HelloWorld::testDrop(LetterTile* letter) {
    auto iter = landings.begin();
    while(iter != landings.end()) {
        Vec2 t = letter->uiPosition();
        t -= landingTray->getPosition();
        Rect r = (*iter)->getBoundingBox();
        if ( r.containsPoint(t) && (*iter)->isVisible() ) {
            Vec2 n = (*iter)->getPosition();
            n += landingTray->getPosition();
            letter->uiPosition(n);
            return true;
        }
        ++iter;
    }
    return false;
}

void HelloWorld::onEnter() {
    Layer::onEnter();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    landingTray = Node::create();
    addChild(landingTray);
    homeTray = Node::create();
    addChild(homeTray);
    
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
            homeTray->addChild(aTile);
        }
        aLanding = TileLanding::create();
        aLanding->correctLetter = aWord[i];
        landings.push_back(aLanding);
        landingTray->addChild(aLanding);
        ++i;
    }
    random_shuffle ( letterTiles.begin(), letterTiles.end() );
    
    int count = letterTiles.size();
    Rect r = aTile->getBoundingBox();
    float lastX = 0;
    for (int j = 0; j < count; ++j) {
        letterTiles[j]->setPosition(lastX + (r.size.width*0.5), 0);
        lastX += r.size.width + 5;
    }
    homeTray->setPosition(Vec2((visibleSize.width*0.5)-(lastX*0.5), 75));
    
    lastX = 0;
    for (int j = 0; j < i; ++j) {
        aLanding = landings[j];
        r = aLanding->getBoundingBox();
        aLanding->setPosition(lastX + (r.size.width*0.5), 0);
        lastX += r.size.width;
    }
    landingTray->setPosition(Vec2((visibleSize.width*0.5)-(lastX*0.5), 255));
}

bool HelloWorld::onTouchBegan(Touch *touch, Event *event) {
    //Vec2 t = touch->getLocation();
    return true;
}

void HelloWorld::onTouchMoved(Touch *touch, Event *event) {
    Vec2 t = touch->getDelta();
}

void HelloWorld::onTouchEnded(Touch *touch, Event *event) {
    //    tray->endMove();
}

void HelloWorld::update(float delta) {

}