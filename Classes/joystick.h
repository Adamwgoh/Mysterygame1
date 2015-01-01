//
//  joystick.h
//  battleout
//
//  Created by Adam on 7/6/14.
//
//

#ifndef __battleout__joystick__
#define __battleout__joystick__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class joystick : public Layer{
public:
    
    virtual bool init();
    void resetclick();
    Point getVelocity(){ return velocity; };

    bool clicked();
    
private:
    int joystick_radius = 64.0f;
    int thumb_radius = 26.0f;

    
    int nofTaps;
    int taps;
    Point kCenter;
    Sprite* thumb;
    Sprite* bg;
    bool isPressed;
    bool click;
    Point velocity;
    
    void updateVelocity(Point point);
    void resetJoystick();
    void checkTaps(float dt);
    
    //joystick configs
    void setJoystickThumb(Sprite* thumb);
    void setJoystickBackground(Sprite* background);
    void setThumbRadius(int thumb_radius);
    void setBackgroundRadius(int bg_radius);
    
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
};
#endif /* defined(__battleout__joystick__) */
