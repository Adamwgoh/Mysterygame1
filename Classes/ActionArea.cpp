//
//  ActionArea.cpp
//  battleout
//
//  Created by Adam on 7/3/14.
//
//

#include "ActionArea.h"
#include "joystick.h"
//TOOD: this is a general class for action area sprites etc. please refactor joystick to another state
//      Have the joystick as the movement extension of this general class.

bool ActionArea::init(){
    box = Sprite::create("actionarea_box.png");
    box->setAnchorPoint(Point(0,0));
    box->setPosition(Point(0,0));
    this->addChild(box);
    box->setZOrder(0);
    //box->setOpacity(0);
    js = new joystick();
    js->init();

    if(js != NULL){
        this->addChild(js);
        return true;
    }
    
    return true;
}

joystick* ActionArea::getJoystick(){
    if(js == NULL){
        log("joystick is not setup");
    }
    
    return js;
}

Sprite* ActionArea::getAAbox(){
    return box;
}

ActionArea* ActionArea::createInstance(){
    ActionArea* obj = new ActionArea();
    return obj;
}

void ActionArea::addInteraction(std::string interaction){
    action_name = Label::createWithTTF(interaction, "Pxlvetica.ttf", 30);
    action_name->setPosition(320,210);
    this->addChild(action_name);
}

void ActionArea::removeInteraction(std::string interaction){
    this->removeChild(action_name);
}

States::State ActionArea::getState(){
    return the_state;
}

void ActionArea::setState(States::State state){
    this->the_state = state;
    this->stateChange = true;
}

bool ActionArea::click(){

    if(js->clicked()){
        js->resetclick();
        return true;
    }
    
    return false;
}

void ActionArea::setParentScene(Layer* scene){
    current_scene = scene;
}

//SpeechAA virtual methods
bool ActionArea::LoadOptionLines(const char* replyfiles_url){    printf("called base-method.\n");    return false; }
void ActionArea::setupMenu(int current_tree_level, int current_choice, int &replyy){     printf("called base SetupMenu method.\n"); }
bool ActionArea::getOptionsStrings(int tree_level){     printf("called base-method.\n"); return false; }
int  ActionArea::getReply(){    printf("called base-method.\n");return -1; }
