//
//  joystick.cpp
//  battleout
//
//  Created by Adam on 7/6/14.
//
//

#include "joystick.h"


#define JOYSTICK_OFFSET_X 5.0f
#define JOYSTICK_OFFSET_Y 5.0f

//#define JOYSTICK_RADIUS 64.0f

//#define THUMB_RADIUS 26.0f

static Point convertCoordinate(Point point){
    return Director::getInstance()->convertToGL(point);
}

static bool isPointInCircle(Point point, Point center, float radius){
    //printf("checking if point is in circle \n");
    float dx = (point.x - center.x);
    float dy = (point.y - center.y);
    return (radius >= sqrt((dx*dx)+(dy*dy)));
}

bool joystick::init()
{
    bool bRet = false;
    do
    {
        this->setTouchEnabled(true);
        Size winSize = Director::getInstance()->getWinSize();
        kCenter= Point(winSize.width/2, 232/2);
        CC_BREAK_IF(!CCLayer::init());
        
        velocity = Point::ZERO;
        
        bg = Sprite::create("joy_background.png");
        //bg->setAnchorPoint(Point(0,0));
        bg->setPosition(kCenter);
        this->addChild(bg,3);
        
        thumb = Sprite::create("joy_center.png");
        thumb->setPosition(kCenter);
        this->addChild(thumb,3);
        
        bRet=true;
 
        EventListenerTouchOneByOne* listener = EventListenerTouchOneByOne::create();
        listener->onTouchBegan = CC_CALLBACK_2(joystick::onTouchBegan, this);
        listener->onTouchEnded = CC_CALLBACK_2(joystick::onTouchEnded, this);
        listener->onTouchMoved = CC_CALLBACK_2(joystick::onTouchMoved, this);
        listener->onTouchCancelled = CC_CALLBACK_2(joystick::onTouchCancelled, this);
        //listener->setSwallowTouches(true);
        auto dispatcher = Director::getInstance()->getEventDispatcher();
        dispatcher->addEventListenerWithSceneGraphPriority(listener, thumb);
        
        
        
        
    } while(0);
    
    return bRet;
}

void joystick::updateVelocity(Point point)
{
    // calculate Angle and length
    float dx = point.x - kCenter.x;
    float dy = point.y - kCenter.y;
    
    float distance = sqrt(dx*dx + dy*dy);
    float angle = atan2(dy,dx); // in radians
    
    if (distance > joystick_radius)
    {
        dx = cos(angle) * joystick_radius;
        dy = sin(angle) * joystick_radius;
    }
    
    //to show that negative values note left/down, while positive note right/up
    velocity = Point(dx/joystick_radius, dy/joystick_radius);
    //printf("velocity x is %f, velocity y is %f", velocity.x, velocity.y);
    
    if(distance>thumb_radius)
    {
        point.x = kCenter.x + cos(angle) * thumb_radius;
        point.y = kCenter.y + sin(angle) * thumb_radius;
    }
    
    thumb->setPosition(point);
    
}

void joystick::onTouchCancelled(Touch* touch, Event* event){
   // printf("touch cancelled\n");
}


bool joystick::onTouchBegan(Touch* touch, Event* event){
    
    //target that was clicked on
    Sprite* target = static_cast<Sprite*>(event->getCurrentTarget());
    //get its location
    Point locationInNode = target->convertToNodeSpace(touch->getLocation());
    
    Size s = thumb->getContentSize();
    Rect rectarea = Rect(0,0, s.width, s.height);

  
    //get thumb rectarea
    if( rectarea.containsPoint(locationInNode) ){

        //to reduce sensitivity on the thumb movement to avoid stutter effect during slight jab touches.
        if(abs(kCenter.x - touch->getLocation().x) > s.width*0.5){
            updateVelocity(touch->getLocation());
        }
        
        isPressed = true;
    }
    
    return true;
}


void joystick::onTouchMoved(Touch* touch, Event* event){
    taps = 0;   //because it moved, no consecutive taps will be counted

        if(isPressed) {
            Size bg_content_size = bg->getContentSize();

            Rect bg_bound = bg->getBoundingBox();
            //get new touch location and move it there
            updateVelocity(touch->getLocation());
        
        }
    
        //if reaches to the thumb area, restart touch and move thumb
        Rect thumb_box = thumb->getBoundingBox();
    
        if(thumb_box.containsPoint(touch->getLocation()) ){
            nofTaps = 0;
            onTouchBegan(touch, event);
        
        }
    
    //printf("%f", velocity.x);
    
    //printf("touch Moved\n");
}


void joystick::onTouchEnded(Touch* touch, Event* event){
    
    //bring joystick thumb back to the original position
    //end touch
    if(isPressed){
         updateVelocity(kCenter);
         isPressed = false;
    }
    if(nofTaps == 0){
        if(!this->isScheduled(schedule_selector(joystick::checkTaps)) ){
            this->schedule(schedule_selector(joystick::checkTaps), 0.25, 1, 0);
        }
    }
    
    if(thumb->getBoundingBox().containsPoint(touch->getLocation()) ){
        nofTaps++;
    }
    if(nofTaps > 2) nofTaps = 0;    //for now only double tap is supported
}

//DONE: the logic here is wrong, but right now it works for double tap
void joystick::checkTaps(float dt){

    this->unschedule(schedule_selector(joystick::checkTaps));

    if(nofTaps == 2){
        nofTaps = 0;
        click = true;
    }else{
        nofTaps = 0;
        click = false;
    }
}

bool joystick::clicked(){
    return click;
}

void joystick::resetclick(){
    click = false;
}
