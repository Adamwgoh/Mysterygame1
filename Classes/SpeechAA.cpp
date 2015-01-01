//
//  SpeechAA.cpp
//  battleout
//
//  Created by Adam on 8/1/14.
//
//

#include "SpeechAA.h"
#include "AnimatedLabel.h"

bool SpeechAA::init(){
    box = Sprite::create("actionarea_box.png");
    this->addChild(box);
    box->setAnchorPoint(Point(0,0));
    box->setOpacity(100);
    box->setPosition(Point(0,0));
    box->setZOrder(0);
    
    
    return true;
}

bool SpeechAA::LoadOptionLines(const char* replyfiles_url){
    std::ifstream instream;
    std::string url = FileUtils::getInstance()->fullPathForFilename(replyfiles_url);
    instream.open(url, std::ios::in);
    std::string line;
    int tree_level;
    int option_no;
    option_tree = new std::multimap<std::pair<int,int>, std::string>();
    if(instream.bad()){ printf("instream : bad\n");}

    
    if(!instream.is_open()){
        printf("file load failed\n");
        return false;
    }else{
        while(instream >> tree_level >> option_no){
            std::getline(instream, line);
            
            std::pair<int,int> pair = std::make_pair(tree_level, option_no);
            printf("%d %d %s\n", tree_level, option_no, line.c_str());
            option_tree->insert(std::multimap<std::pair<int,int>, std::string>::value_type(pair,line) );
        }
        printf("options size : %lu\n", option_tree->size());
        printf("gotten lines\n");
        return true;
    }
}

std::vector<std::string>* SpeechAA::getOptionsStrings(int tree_level, int choice_level){
    printf("getting option strings\n");
    typedef std::multimap<std::pair<int,int>,std::string>::iterator OptionItr;
    std::vector<std::string>* option_lines = new std::vector<std::string>();
    
    std::pair<OptionItr,OptionItr> option_range = option_tree->equal_range(std::make_pair(tree_level,choice_level) );
    for(OptionItr itr = option_range.first; itr != option_range.second; ++itr){
        if((*itr).first.first == tree_level && (*itr).first.second == choice_level){
            option_lines->push_back((*itr).second);
        }
    }
    
    return option_lines;
    
}

int SpeechAA::getReply(){      return *reply; }

bool SpeechAA::onTouchBegan(Touch* touch, Event* event){

    return  true;
}

SpeechAA* SpeechAA::createInstance(){
    SpeechAA* obj = new SpeechAA();
    return obj;
}


void SpeechAA::MenuCallback(Object* ref){
    //just a test
    MenuItem* item = (MenuItem*) ref;
    
    int tag = item->getTag();
    switch (tag) {
            
        case 100:
            this->setState(States::DEFAULT);  *reply = 0;
            break;
        case 101:
            this->setState(States::DEFAULT); *reply = 1;
            break;
        case 102:
            this->setState(States::DEFAULT);  *reply = 2;
            break;
        case 103:
            this->setState(States::DEFAULT);  *reply = 3;
            break;
        case 104:
            this->setState(States::DEFAULT);  *reply = 4;
            break;
        case 105:
            this->setState(States::DEFAULT);  *reply = 5;
            break;
        default:    *reply = -1;
            break;
    }


}

void SpeechAA::setupMenu(int current_tree_level, int current_choice, int &replyy){
    printf("setting up menu..\n");
    
    Menu* menu = Menu::create();
    menu->setContentSize(box->getContentSize());
    menu->setAnchorPoint(Point(0,0.5));
    reply = &replyy;
    if(!option_tree->empty()){
        int tag = 100;
        std::vector<std::string>* options = getOptionsStrings(current_tree_level, current_choice);
        std::vector<MenuItem*>* option_menuitems = new std::vector<MenuItem*>();
        
        for(std::string option : *options){
            Label* label = Label::createWithTTF(option, "Mockup-Regular.otf", 17);
//            label->setScale(0.25);
            printf("label : %s\n", label->getString().c_str());
            MenuItem* menuitem = MenuItem::create();
            menuitem = MenuItemLabel::create(label, CC_CALLBACK_1(SpeechAA::MenuCallback, this));
            menuitem->setTag(tag);
            menu->addChild(menuitem);
            //option_menuitems->push_back(menuitem);
            tag++;
        }

        for(MenuItem* item : *option_menuitems){
            menu->addChild(item);
            item->setAnchorPoint(Point(0,0));
        }
    }
    
    menu->setColor(Color3B::WHITE);
    menu->setPosition(box->getContentSize().width/5,box->getContentSize().height/2);
    menu->alignItemsVerticallyWithPadding(0);
    
    box->addChild(menu);

    /**
     *  Removed exit button. Design decision. Not neccesary to have exit button during conversation. 
     **/
//    //menu 2 to put exit button on the other end
//    
//    Menu* exitbutton = Menu::create();
//    exitbutton->setContentSize(box->getContentSize());
//    exitbutton->setAnchorPoint(Point(0,0.5));
//    
//    Label* exit = Label::createWithTTF("Back", "Pxlvetica.ttf", 40);
//    MenuItem* exit_label = MenuItemLabel::create(exit, CC_CALLBACK_1(SpeechAA::ExitMenuCallback,this));
//    exit_label->setTag(404);
//    exitbutton->addChild(exit_label);
//    exitbutton->setPosition(box->getContentSize().width - exit_label->getContentSize().width, box->getContentSize().height/2);
//
//    box->addChild(exitbutton);

}

/**********
 *  Removed exit button. Design decision. Not neccesary to have exit button during conversation. 
 ***********
void SpeechAA::ExitMenuCallback(Object* pSender){
    MenuItem* ref = (MenuItem*) pSender;
    
    if(ref->getTag() == 404){
        this->setState(States::DEFAULT);
        *reply = -1;

        
    }
}
**/


void SpeechAA::onTouchMoved(Touch* touch, Event* event){

}

void SpeechAA::onTouchEnded(Touch* touch, Event* event){

}

void SpeechAA::onTouchCancelled(Touch* touch, Event* event){

}