#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

#define DEBUG_RESTART 0

static cocos2d::Size mediumResolutionSize = cocos2d::Size(960, 640);

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs() {
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    std::vector<std::string> searchPaths;
    searchPaths.push_back("assets");
    
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("Evil", Rect(0, 0, mediumResolutionSize.width, mediumResolutionSize.height));
#else
        glview = GLViewImpl::create("Evil");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    FileUtils::getInstance()->setSearchPaths(searchPaths);
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("assets.plist");

    reloadScene(NULL);
    
    return true;
}

void AppDelegate::reloadScene(Ref* pSender) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    auto director = Director::getInstance();
    
    // create a scene. it's an autorelease object
    auto scene = HelloWorld::createScene();
    
    director->runWithScene(scene);
    
#if DEBUG_RESTART
    //add a "reload" button to see the game start animation over again
    ////
    auto restartItem = MenuItemImage::create();//"restart.jpg", "restart.jpg", CC_CALLBACK_1(AppDelegate::reloadScene, this));
    restartItem->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("restart.jpg"));
    restartItem->setCallback( CC_CALLBACK_1(AppDelegate::reloadScene, this) );
    restartItem->setScale(0.25);
    
    // create menu to hold the restart button
    auto menu = Menu::create(restartItem, NULL);
    menu->setPosition(Vec2(25, visibleSize.height-25));
    scene->addChild(menu, 10);
#endif
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
