//
//  CountDown.hpp
//  Evil
//
//  Created by Jason on 2/1/16.
//
//

#ifndef CountDown_hpp
#define CountDown_hpp

#include "cocos2d.h"

USING_NS_CC;

class HelloWorld;

class CountDown : public Node {
public:
    HelloWorld* delegate;
    int count;
    Label* label;
    
    CREATE_FUNC(CountDown);

    virtual void onEnter();
    void updateTick(float delta);
    void stop();
};

#endif /* CountDown_hpp */
