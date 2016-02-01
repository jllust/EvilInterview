//
//  CountDown.cpp
//  Evil
//
//  Created by Jason on 2/1/16.
//
//

#include "CountDown.hpp"
#include "HelloWorldScene.h"
#include "audio/include/SimpleAudioEngine.h"
#include <stdio.h>

void CountDown::onEnter() {
    Node::onEnter();
    count = 59;
    label = Label::createWithBMFont("countdown0.fnt", "59");
    addChild(label);
    
    schedule(CC_SCHEDULE_SELECTOR(CountDown::updateTick), 1, count-1, 0);
}

void CountDown::updateTick(float delta) {
    char t[5];
    sprintf(t, "%i", --count);
    label->setString(t);
    
    if (count == 10) {
        float shake = 0.5;
        float remained = 1 - shake;
        auto move1 = MoveBy::create(shake, Vec2(8, 0));
        auto move2 = MoveBy::create(shake, Vec2(-16, 0));
        auto ease1 = EaseElasticOut::create(move1);
        auto ease2 = EaseElasticOut::create(move2);
        auto repeat = Repeat::create(Sequence::create(ease1, DelayTime::create(remained), ease2, DelayTime::create(remained), NULL), 5);
        runAction(repeat);
        label->setColor(Color3B(255, 0, 0));
        CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("sounds/mj_lastplace.caf");
    }
    
    if (count < 1) {
        label->setString("0");
        unscheduleAllCallbacks();
        delegate->outOfTime();
    }
}

void CountDown::stop() {
    unscheduleAllCallbacks();
}

