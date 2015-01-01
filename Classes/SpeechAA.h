//
//  SpeechAA.h
//  battleout
//
//  Created by Adam on 8/1/14.
//
//

#ifndef __battleout__SpeechAA__
#define __battleout__SpeechAA__

#include <iostream>
#include <fstream>
#include "cocos2d.h"

#include "ActionArea.h"

using namespace cocos2d;

class SpeechAA : public ActionArea{
public:
    bool init();
    static SpeechAA* createInstance();
    
    virtual std::vector<std::string>* getOptionsStrings(int tree_level, int choice_level);
    virtual bool LoadOptionLines(const char* replyfiles_url);
    virtual int  getReply();
    virtual void setupMenu(int current_tree_level, int current_choice, int &reply);
private:
    int* reply;
    
    void ExitMenuCallback(Object* pSender);
    void MenuCallback(Object* ref);
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void onTouchCancelled(Touch* touch, Event* event);
    std::multimap<std::pair<int,int>, std::string>* option_tree;
    //std::multimap<int, std::multimap<int,std::string>>* option_tree;
    //std::multimap<int, std::string>* option_lines_stack;
    //std::vector<std::string>* options;
    
};
#endif /* defined(__battleout__SpeechAA__) */
