//
//  Conversation.h
//  battleout
//
//  Created by Adam on 8/22/14.
//
//

#ifndef __battleout__Conversation__
#define __battleout__Conversation__

#include <iostream>
#include <fstream>
#include "cocos2d.h"
#include "ActionArea.h"
#include "AnimatedLabel.h"

namespace SpokePerson{
    
    enum SpokePerson{ PLAYER, NPC };
}

class Conversation
{
public:
    void init(Layer* scene,const char* filename, const char* npc, int tree_level, int choice);
    void init(Layer* scene,const char* filename,const char* reply_filename, const char* npc, int tree_level, int choice);
    bool loadXml(const char* xml_url);
    bool loadConvoFile(const char* filename, SpokePerson::SpokePerson person);

    bool isConvoEnded();
    bool isConvoStarted();
    bool isWaitingReply();
    void cleanup();

    
private:
    //touch handlers
    EventListenerTouchOneByOne* listener;
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);

    //speech handlers
    std::vector<std::string>* getConvoLines(int tree_level, int option_no, SpokePerson::SpokePerson person);
    void showSpeech(std::string speech, SpokePerson::SpokePerson person);
    void callSpeech(int tree_level, int option_no, int linecount, SpokePerson::SpokePerson person);
    void setSpeechPosition(AnimatedLabel* sentence, SpokePerson::SpokePerson person);
    void setSpeechPosition(AnimatedLabel* sentence, SpokePerson::SpokePerson person, Label* char_name);
    
    //variables
    std::string npc_name;
    Sprite* continue_button;
    Layer* current_scene;
    Animation* cont_animation;
    Animate* animate_cont;
    RepeatForever* contbutton_RF;
    std::multimap<std::pair<int,int>,std::string>* convo_tree;
    std::multimap<std::pair<int,int>,std::string>* reply_tree;
    AnimatedLabel* sentence;

    
    //state bools
    bool convo_end = false;
    bool waiting_reply = false;
    bool convo_started = false;
    bool animation_end = true;
    
    
    int  current_choice = -1;
    int  current_tree_level = -1;
    int  current_linecount = -1;
    SpokePerson::SpokePerson current_person;
};
#endif /* defined(__battleout__Conversation__) */
