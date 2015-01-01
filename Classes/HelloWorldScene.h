#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "Character.h"
#include "NonPlayer.h"
#include "joystick.h"
#include "ActionArea.h"
using namespace cocos2d;


//TODO: can be used to be inherited again
class HelloWorld : public Layer
{
public:
    
    Character* torc;
    Size getMapSize();
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    void update(float dt);
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    void setScenePause(bool paused);
    bool getScenePause();
    void moveMap(Point velocity, int direction);
    bool setPerspective(int direction);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    
private:
    b2World* _world;
    ActionArea* action_area;
    Sprite* main_char;
    NonPlayer* npc;
    Sprite* new_map;
    Point current_playview;
    bool scene_paused = false;
    
    //scene state bools. Will add more with features to come
    bool moving = false;
    bool mapScroll = false;
    Vector<Touch*>* _touches;
    
    void callPause(Object* pSender);
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    
    void onTouchesBegan(std::vector<Touch*> touches, Event* event);
    void onTouchesMoved(std::vector<Touch*> touches, Event* event);
    void onTouchesEnded(std::vector<Touch*> touches, Event* event);
    void onTouchesCancelled(std::vector<Touch*> touches, Event* event);
    void ScrollMap(float dx, float dy);
    bool checkScrolling();
    void resetView();
    void moveCamera();
    
    void setBoundedPosition(Point position, Node* obj);

};

#endif // __HELLOWORLD_SCENE_H__
