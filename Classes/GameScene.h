//
//  GameScene.h
//  battleout
//
//  Created by Adam on 8/10/14.
//
//

#ifndef __battleout__GameScene__
#define __battleout__GameScene__

#include <iostream>
#include "cocos2d.h"
#include "ActionArea.h"
#include "SpeechAA.h"
#include "AnimatedLabel.h"
#include "PauseLayer.h"
#include "Character.h"


using namespace cocos2d;

class GameScene : public Layer{

public:
    Character* main_char;
    
    static Scene* createScene();
    virtual void update(float dt);
    virtual bool init();
    
    void moveMap(Point speed, int direction);
    bool setPerspective(int direction);

    
    
protected:
    Sprite* current_map;
    ActionArea* action_area;
    Point current_playview;
    
    //scene state bools.
    bool scene_paused;
    bool moving = false;
    bool mapScroll = false;
    
    void callPause(Object* pSender);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void ScrollMap(float dx, float dy);
    bool checkScrolling();
    void resetView();
    void moveCamera();
    
    void setupAA();
    void setupMap(const char* map_filename);
    void setupTouchlistener();
    void setupPause();
    
};
#endif /* defined(__battleout__GameScene__) */
