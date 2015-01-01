//
//  PauseLayer.cpp
//  battleout
//
//  Created by Adam on 8/7/14.
//
//

#include "PauseLayer.h"

bool PauseLayer::inflate(){
    if(!Layer::init()){
        return false;
    }
    
    Size winSize = Director::getInstance()->getWinSize();
    auto bg_layer = LayerColor::create(Color4B::BLACK, winSize.width, winSize.height);
    bg_layer->setPosition(Point::ZERO);
    bg_layer->setOpacity(95);
    

    this->addChild(bg_layer);
    //map touch listeners
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(PauseLayer::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(PauseLayer::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(PauseLayer::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(PauseLayer::onTouchCancelled, this);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    listener->setSwallowTouches(true);
    dispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    this->setZOrder(3);
    setupPauseMenu();
    
    return true;
}

void PauseLayer::setParentScene(Layer* scene){
    current_scene = scene;
}

//TODO: click twice to resume again, joystick broke after pause
void PauseLayer::setupPauseMenu(){
    Size winSize = Director::getInstance()->getWinSize();
    Label* resume = Label::createWithTTF("Resume", "Pxlvetica.ttf", 40);
    MenuItem* resume_button = MenuItemLabel::create(resume, CC_CALLBACK_1(PauseLayer::cancelPause, this));
    Menu* menu = Menu::create(resume_button, NULL);
    menu->setPosition(winSize.width/2, winSize.height/2);
    
    this->addChild(menu);
}

void PauseLayer::cancelPause(Object* pSender){
    
    current_scene->removeChild(this);
}

bool PauseLayer::onTouchBegan(Touch* touch, Event* event){
    printf("pause layer touched\n");
    return true;
}
void PauseLayer::onTouchMoved(Touch* touch, Event* event){}
void PauseLayer::onTouchEnded(Touch* touch, Event* event){}