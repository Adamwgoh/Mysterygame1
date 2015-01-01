//
//  AnimatedLabel.cpp
//  This is for all future animations regarding texts
//
//  Created by Adam on 7/28/14.
//
//

#include "AnimatedLabel.h"

AnimatedLabel* AnimatedLabel::create(std::string str, const char* filename, int fontSize){
    AnimatedLabel* ani_label = new AnimatedLabel();
    
    if(ani_label->createWithTTF(str, filename, fontSize)){
        ani_label->autorelease();
        return (AnimatedLabel*) ani_label->createWithTTF(str, filename, fontSize);
    }
    
    CC_SAFE_DELETE(ani_label);
    return NULL;
}

void AnimatedLabel::setAllCharScale(float ds){
    int str_length = getStringLength();
    
    for( int i = 0; i < str_length; i++){
        Sprite* letter = this->getLetter(i);

        if(letter != nullptr){
            letter->setScale(ds);
        }
    }
}

void AnimatedLabel::setMaxSentenceWidth(int width){
    this->setMaxLineWidth(width);
}

void AnimatedLabel::runActionOnAllSpritesReversedSequentially(Action* action, float duration){

}

void AnimatedLabel::runActionOnAllSpritesSequentially(Action* action, float duration){

    int str_length = this->getStringLength();
    
    for (int i = 0; i < str_length; i++){
        DelayTime* start_delay = DelayTime::create(duration);
        DelayTime* delay = DelayTime::create((duration/(str_length-1)) * ((str_length-1)+i) );
        Action* action_copy = action->clone();
        
        Sequence* initial_delayedAction = Sequence::create(start_delay, action_copy, NULL);
        Sequence* delayed_action = Sequence::create(delay, action_copy, NULL);
        Sprite* charSprite = (Sprite*) this->getLetter(i);
        
        
        if (i == 0 ){   //first letter doesn't delay
            if(charSprite != nullptr){
                charSprite->runAction(initial_delayedAction);
            }
        }else{
            if(charSprite != nullptr){
                charSprite->runAction(delayed_action);
            }
        }
        
    }
}

void AnimatedLabel::AnimateTypewrite(float duration){
    setAllCharScale(0);
    auto scale = ScaleTo::create(0, 1);
    runActionOnAllSpritesSequentially(scale, duration);
    
}

