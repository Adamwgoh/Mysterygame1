//
//  AnimatedLabel.h
//  battleout
//
//  Created by Adam on 7/28/14.
//
//

#ifndef __battleout__AnimatedLabel__
#define __battleout__AnimatedLabel__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class AnimatedLabel : public Label{

public:
    CREATE_FUNC(AnimatedLabel);
    static AnimatedLabel* create(std::string str, const char* filename, int fontSize);
    
    void setAllCharScale(float ds);
    
    void AnimateTypewrite(float duration);
    void setMaxSentenceWidth(int width);
    void runActionOnAllSpritesSequentially(Action* action, float duration);
    void runActionOnAllSpritesReversedSequentially(Action* action, float duration);
    
protected:
    
private:
    
};
#endif /* defined(__battleout__AnimatedLabel__) */
