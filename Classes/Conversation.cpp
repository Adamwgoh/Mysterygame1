//
//  Conversation.cpp
//  battleout
//
//  Created by Adam on 8/22/14.
//
//

#include "Conversation.h"

void Conversation::init(Layer* scene,const char* filename, const char* npc, int tree_level, int choice){
    npc_name = npc;
    current_scene = scene;
    current_tree_level = tree_level;
    current_choice = choice;
    current_linecount = 0;
    convo_tree = new std::multimap<std::pair<int, int>, std::string>();
    
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Conversation::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Conversation::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Conversation::onTouchEnded, this);
    listener->setSwallowTouches(true);

    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //check on the priority level. may induce bug
    dispatcher->addEventListenerWithFixedPriority(listener, 1);

    if(loadConvoFile(filename, SpokePerson::NPC)){
        printf("file loaded\n");
        callSpeech(current_tree_level, current_choice, current_linecount, SpokePerson::NPC);
        convo_started = true;
        
    }else{        printf("file not loaded\n");}
}

void Conversation::init(Layer* scene,const char* filename, const char* reply_filename, const char* npc, int tree_level, int choice){
    npc_name = npc;
    current_scene = scene;
    current_tree_level = tree_level;
    current_choice = choice;
    current_linecount = 0;
    convo_tree = new std::multimap<std::pair<int, int>, std::string>();
    
    listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Conversation::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(Conversation::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(Conversation::onTouchEnded, this);
    listener->setSwallowTouches(true);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //check on the priority level. may induce bug
    dispatcher->addEventListenerWithFixedPriority(listener, 1);
    
    if(loadConvoFile(reply_filename, SpokePerson::PLAYER)){
        printf("reply file loaded\n");
        current_person = SpokePerson::PLAYER;
        callSpeech(current_tree_level, current_choice, current_linecount, SpokePerson::PLAYER);
        convo_started = true;
    }else{  printf("reply file not loaded\n");}
    
    if(loadConvoFile(filename, SpokePerson::NPC)){
        printf("file loaded\n");
        current_person = SpokePerson::NPC;
        if(!convo_started){
            callSpeech(current_tree_level, current_choice, current_linecount, SpokePerson::NPC);
            convo_started = true;
        }else if(convo_started && current_person ){
        
        }
        
    }else{  printf("file not loaded\n");}
    
}

bool loadXml(const char* xml_url){

    
    return true;
}

void Conversation::cleanup(){
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->removeEventListener(listener);
    dispatcher->release();
    
    convo_end = false;
    waiting_reply = false;
    convo_started = false;
    animation_end = true;
    
}

bool Conversation::loadConvoFile(const char* filename, SpokePerson::SpokePerson person){
    //loads from a txt file
    std::ifstream instream;
    std::string file_url = FileUtils::getInstance()->fullPathForFilename(filename);
    instream.open(file_url, std::ios::in);
    std::string line;
    std::string result;
    int tree_level;
    int option_no;
    convo_tree = new std::multimap<std::pair<int,int>, std::string>();
    reply_tree = new std::multimap<std::pair<int,int>, std::string>();
    
    if(!instream.is_open()){

        printf("falsee\n");
        return false;
    }else{
        printf("Stream successfully open\n");
        if(instream.bad())  printf("bad\n");
        if(instream.fail()) printf("fail\n");
        
        while(instream >> tree_level >> option_no){
            //get line
            std::getline(instream, line);
            std::pair<int,int> pair = std::make_pair(tree_level, option_no);
            //Each pair in line stack is a choice option number and its string
            if(person == SpokePerson::NPC){
                convo_tree->insert(std::multimap<std::pair<int,int>, std::string>::value_type(pair, line) );
            }else if(person == SpokePerson::PLAYER){
                reply_tree->insert(std::multimap<std::pair<int,int>, std::string>::value_type(pair, line) );
            }
            
        }
        instream.close();
        instream.clear();
        
        return true;
    }
    
    return false;
}

std::vector<std::string>* Conversation::getConvoLines(int tree_level, int option_no, SpokePerson::SpokePerson person){
    typedef std::multimap<std::pair<int,int>,std::string>::iterator TreeItr;
    TreeItr itr;
    std::vector<std::string>* convo_lines = new std::vector<std::string>();
    std::pair<TreeItr,TreeItr> convo_range;
    if(SpokePerson::NPC){
            convo_range = convo_tree->equal_range(std::make_pair(tree_level, option_no));
    }else if(SpokePerson::PLAYER){
            convo_range = reply_tree->equal_range(std::make_pair(tree_level, option_no));
    }
        
    
    for(TreeItr j = convo_range.first; j != convo_range.second; ++j){
        if((*j).first.first == tree_level && (*j).first.second == option_no){
            convo_lines->push_back((*j).second);
        }
    }
    
    return convo_lines;
}

void Conversation::showSpeech(std::string speech, SpokePerson::SpokePerson person){
    animation_end = false;
    sentence = AnimatedLabel::create(speech, "Mockup-Bold.otf", 30);
    Label* name_label;
    if(npc_name != ""){
        const std::string name = npc_name;
        name_label = Label::createWithTTF(name, "Mockup-Bold.otf", 40);
        name_label->setColor(Color3B::RED);
        name_label->setMaxLineWidth(200);
        current_scene->addChild(name_label);
        npc_name = "";
        
    }
    sentence->setMaxSentenceWidth(200);
    sentence->setColor(Color3B::BLACK);
    sentence->setAlignment(TextHAlignment::RIGHT, TextVAlignment::CENTER);
    current_scene->addChild(sentence);
    if(name_label != nullptr && person == SpokePerson::NPC){
        
        setSpeechPosition(sentence, person, name_label);
    }else if(person == SpokePerson::PLAYER){
        
        setSpeechPosition(sentence,person);
    }
    sentence->AnimateTypewrite(1);
    auto hangaround = RepeatForever::create(DelayTime::create(1));
    
    sentence->runAction(hangaround);
    animation_end = true;
    
    
}

//access speech file to get the right speech to show
void Conversation::callSpeech(int tree_level, int option_no, int linecount, SpokePerson::SpokePerson person){
    printf("calling speech\n");
    std::vector<std::string>* Lines = new std::vector<std::string>();
    printf("tree level : %d", tree_level);
    if(person == SpokePerson::NPC){
        Lines = getConvoLines(tree_level, option_no, person);
    }else if(person == SpokePerson::PLAYER){
        Lines = getConvoLines(tree_level, option_no, person);
    }

    showSpeech(Lines->at(linecount), person);
    printf("exit call speech\n");
    if(linecount == Lines->size()-1){//end of stack
        printf("ends convo\n");
        current_linecount = -1;
    }
}

void Conversation::setSpeechPosition(AnimatedLabel* sentence, SpokePerson::SpokePerson person){
    if(person == SpokePerson::PLAYER){
        sentence->setPosition(Director::getInstance()->getWinSize().width-100,800);
    }else if(person == SpokePerson::NPC){
        //set diff area
        sentence->setPosition(Director::getInstance()->getWinSize().width-100,800);
    }
    
}

void Conversation::setSpeechPosition(AnimatedLabel* sentence, SpokePerson::SpokePerson person, Label* char_name){
    if(person == SpokePerson::PLAYER){
        sentence->setPosition(Director::getInstance()->getWinSize().width-100,800);
    }else if(person == SpokePerson::NPC){
        //set diff area
        char_name->setPosition(Director::getInstance()->getWinSize().width-100,870);
        sentence->setPosition(Director::getInstance()->getWinSize().width-100,800);
    }
}

bool Conversation::isConvoEnded(){      return convo_end;      }
bool Conversation::isConvoStarted(){    return convo_started;  }
bool Conversation::isWaitingReply(){    return waiting_reply;  }

bool Conversation::onTouchBegan(Touch* touch, Event* event){
    printf("touch\n");
    printf("animation_end : %d\n",animation_end);
    printf("convo_end : %d\n",convo_end);
    printf("convo_started : %d\n",convo_started);
    
    if(!animation_end){
        printf("animation ended\n");
        sentence->stopAllActions();
        animation_end = true;
    }else if(animation_end && !convo_end && convo_started && current_linecount != -1){

        //run next line
        printf("run next line\n");
        current_linecount++;
        current_scene->removeChild(sentence);
        callSpeech(current_tree_level, current_choice, current_linecount, current_person);
        
        
    }else if(animation_end && !convo_end && current_linecount == -1){

        //check if needs reply, else kill conversation
        //restart linecount to kill conversation
        printf("convo killed");
        current_scene->removeChild(sentence);
        convo_end = true;

        
        if(convo_tree->size() > 1){
            printf("current tree level : %d\n", current_tree_level);
            //check if end of tree
            typedef std::multimap<std::pair<int,int>,std::string>::iterator Itr;
            for(Itr i = convo_tree->begin(); i != convo_tree->end(); ++i){
                if((*i).first.first > current_tree_level){
                    waiting_reply = true;printf("gonnna wait\n");
                }else{
                    waiting_reply = false;printf("kthxbai\n");
                }
            }
        }
    }
        
        
    return true;
}

void Conversation::onTouchMoved(Touch* touch, Event* event){

}

void Conversation::onTouchEnded(Touch* touch, Event* event){

}

void Conversation::onTouchCancelled(Touch* touch, Event* event){

}