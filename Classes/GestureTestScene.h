//
//  GestureTestScene.h
//  battleout
//
//  Created by Adam on 9/5/14.
//
//

#ifndef __battleout__GestureTestScene__
#define __battleout__GestureTestScene__

#include <iostream>
#include "cocos2d.h"
#include "GeometricTypes.h"
#include "TemplateRecognizer.h"

using namespace cocos2d;

class GestureTestScene : public LayerColor{
public:
    static cocos2d::Scene* createScene();
    void update(float dt);
    virtual bool init();
    
    
    CREATE_FUNC(GestureTestScene);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);

private:
    SpriteBatchNode* paint;
    typedef std::vector<Touch*> Touches;
    DollarRecognizer::TemplateRecognizer* translator;
    DollarRecognizer::Path2D* gesture;
    //Touches* gesture;
};
#endif /* defined(__battleout__GestureTestScene__) */
