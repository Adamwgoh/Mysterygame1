//
//  object.h
//  battleout
//
//  Created by Adam on 7/12/14.
//
//

#ifndef __battleout__object__
#define __battleout__object__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class object : public Sprite{
public:
    void setMovingAnimation(const char* sprite_batch, const char* moving_plist, int nof_frames);
    void setIdleAnimation(const char* sprite_batch, const char* idle_plist, int nof_frames);
    
protected:
    Sprite* main_sprite;
    const char* char_name;
    int state;
    int direction = 5;
    Point velocity;
    
    RepeatForever *actionStateDefault;
    RepeatForever *actionStateMoving;
    Animate *animateDefault;
    Animate *animateMoving;
    Animation *animationDefault;
    Animation *animationMoving;
    SpriteBatchNode *batchNode;
    
    
private:

};

#endif /* defined(__battleout__object__) */
