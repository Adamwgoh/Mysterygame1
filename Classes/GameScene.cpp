//
//  GameScene.cpp
//  battleout
//
//  Created by Adam on 8/10/14.
//
//

#include "GameScene.h"

void GameScene::setupAA(){
    //adds actionArea onto scene
    this->setAnchorPoint(Point(0,0));
    action_area = new ActionArea();
    action_area->init();
    action_area->setParentScene(this);
    this->addChild(action_area);
}

void GameScene::setupMap(const char* map_filename){
    //loads map
    current_map = Sprite::create(map_filename);
    current_map->setAnchorPoint(Point(0,0));
    current_map->setPosition(Point(0,232));
    current_map->setZOrder(0);
    current_playview = current_map->getPosition();
    this->addChild(current_map);
}

//do not call this if you intend to override the touches functions here
void GameScene::setupTouchlistener(){
    //map touch listeners
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(GameScene::onTouchCancelled, this);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener->setSwallowTouches(true);
    
    dispatcher->addEventListenerWithSceneGraphPriority(listener, current_map);
    
}


void GameScene::setupPause(){
    //pause button
    Label* pause_button = Label::createWithTTF("PAUSE", "Pxlvetica.ttf", 30);
    MenuItem* pause = MenuItemLabel::create(pause_button, CC_CALLBACK_1(GameScene::callPause, this));
    Menu* pauseMenu = Menu::create(pause, NULL);
    pauseMenu->setPosition(500,900);
    pauseMenu->setZOrder(1);
    this->addChild(pauseMenu);
    
}

void GameScene::moveMap(Point speed, int direction){
    Size winSize = Director::getInstance()->getWinSize();
    Size map_size = current_map->getContentSize();
    Point current_position = main_char->getPosition();
    Point new_position = Point(current_map->getPosition().x - (speed.x),current_map->getPosition().y);
    
    if(new_position.x > 0)  new_position.x = 0;
    if(new_position.x < (- map_size.width + winSize.width))     new_position.x = - map_size.width + winSize.width;
    
    //if at the last 10% of the map, do not move
    if(current_position.x/map_size.width < 0.1 || current_position.x/map_size.width > 0.87){
        
        return;
    }else{
        current_map->setPosition(new_position);
    }
}

bool GameScene::setPerspective(int direction){
    //going right is -1, going left is +1
    //left space from side to character and rights as well
    float x_corner = abs(current_map->getPositionX());
    float y_corner = abs(current_map->getPositionX()) + 640;
    
    float left_space = main_char->getCoordX() - x_corner;
    float right_space = y_corner - main_char->getCoordX();
    
    //4 is left, 5 is right
    if(direction == 5){
        //check which side has more, if left had more, swap
        if((left_space > right_space && left_space != right_space) || left_space/right_space > 0.33){
            moveMap(Point(+17, 0), 5);
        }
        
    }else if(direction == 4){
        //same thing happens here as well
        if((right_space  > left_space && right_space != left_space) || right_space/left_space > 0.33){
            current_map->setPositionX(current_map->getPositionX() + 1);
            moveMap(Point((-17), 0), 4);
        }
        
    }
    
    return true;
}

bool GameScene::onTouchBegan(Touch* touch, Event* event){
    if(scene_paused){
        return false;
    }
    //get touch location
    Point location = touch->getLocation();
    
    Rect act_box = action_area->box->getBoundingBox();
    if(act_box.containsPoint(touch->getLocation())){
        mapScroll = false;
        //printf("touch is in action area");
        return false;
    }
    return true;
}
void GameScene::onTouchMoved(Touch* touch, Event* event){
    //write new method to put this, put boundaries, set its z value to below action area
    Point prev_location = touch->getPreviousLocation();
    Point current_location = touch->getLocation();
    mapScroll = true;
    
    auto dx = current_location.x - prev_location.x;
    auto dy = current_location.y - prev_location.y;
    ScrollMap(dx, dy);
}

void GameScene::onTouchEnded(Touch* touch, Event* event){

}

void GameScene::onTouchCancelled(Touch* touch, Event* event){

}

void GameScene::callPause(Object* pSender){
    //create new Pause Layer
    if(!scene_paused && this->getChildByTag(555) == NULL){
        printf("click\n");
        PauseLayer* pause_layer = new PauseLayer();
        pause_layer->inflate();
        pause_layer->setParentScene(this);
        this->addChild(pause_layer);
        this->setZOrder(3);
        pause_layer->setTag(555);
        scene_paused = true;
    }
}

void GameScene::ScrollMap(float dx, float dy){
    
    if(current_map->getPositionX() + dx < 0 &&
       (current_map->getPositionX() + dx > (-current_map->getContentSize().width) + 640) ){
        
        current_map->setPositionX(current_map->getPositionX() + dx);
    }else if(current_map->getPositionY() + dy > (current_map->getContentSize().height-904)  &&
             (current_map->getPositionY() + dy) < (-current_map->getContentSize().height+904)){
        
        current_map->setPositionY(current_map->getPositionY() + dy);
    }
}

bool GameScene::checkScrolling(){
    if(mapScroll)
        return false;
    
    return true;

}

void GameScene::resetView(){
    current_map->setPosition(current_playview);
}

void GameScene::moveCamera(){
    Point velocity = action_area->getJoystick()->getVelocity();
    resetView();
    //from right turning to left
    if(main_char->getDirection() == 5 && (main_char->getCoordX()/current_map->getContentSize().width < 0.87)){
        //set for left
        //printf("setting right\n");
        setPerspective(5);
    }
    //from left turning to right
    else if(main_char->getDirection() == 4 && (main_char->getCoordX()/current_map->getContentSize().width > 0.1)){
        // printf("setting left\n");
        setPerspective(4);
    }
    
    main_char->move(velocity, current_map->getContentSize());
    moveMap(velocity*10, main_char->getDirection());
    current_playview = current_map->getPosition();
}