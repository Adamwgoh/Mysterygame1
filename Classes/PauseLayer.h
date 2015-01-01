//
//  PauseLayer.h
//  battleout
//
//  Created by Adam on 8/7/14.
//
//

#ifndef __battleout__PauseLayer__
#define __battleout__PauseLayer__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class PauseLayer : public Layer{
public:
    virtual bool inflate();
    void setupPauseMenu();
    void cancelPause(Object* pSender);
    void setParentScene(Layer* scene);
    
private:
    Layer* current_scene;
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
};
#endif /* defined(__battleout__PauseLayer__) */
