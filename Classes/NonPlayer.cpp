//
//  NonPlayer.cpp
//  battleout
//
//  Created by Adam on 7/14/14.
//
//
//DONE: 1) A non-player can be approached to start a conversation
//TODO: 2) A non-player can receive a reply from action_area for further responses
//TODO: 3) A non-player will face the Player when being approached for conversation
//TODO: 4) When a conversation is started, a speech dialog will appear
//TODO: 5) Non-player is able to retrieve conversation database as well as animation database from file
//DONE: 6) acknowledges button from action area and initiate conversation action_area
#include "NonPlayer.h"

#define RUNNING_SPEED 10
#define IDLE 1
#define MOVE 2
#define INTERACT 3
#define LEFT 4
#define RIGHT 5

//TODO: things to initiate

bool NonPlayer::init(const char* sprite_filename, const char* cfilename, const char* optfilename, float x, float y,
                     Layer* scene_layer, Character* mainC, Sprite* map){
    batchNode = SpriteBatchNode::create(sprite_filename);
    current_scene = scene_layer;
    layer_map = map;
    mainChar = mainC;
    //convofile_url = "/Users/qwerty/Documents/battleout/Resources/ConvoLines.txt";
    convofile_url  = cfilename;
    optionfile_url = optfilename;
    main_sprite = Sprite::create(sprite_filename);
    main_sprite->setPosition(Point(x, y));
    main_sprite->setAnchorPoint(Point(0,0));
    batchNode->addChild(main_sprite);
    
    return true;
}

bool NonPlayer::init(const char* sprite_filename, const char* convo_filename, float x, float y,
                     Layer* scene_layer, Character* mainC, Sprite* map){
    batchNode = SpriteBatchNode::create(sprite_filename);
    current_scene = scene_layer;
    layer_map = map;
    mainChar = mainC;
    //convofile_url = "/Users/qwerty/Documents/battleout/Resources/ConvoLines.txt";
    convofile_url = convo_filename;
    main_sprite = Sprite::create(sprite_filename);
    main_sprite->setPosition(Point(x, y));
    main_sprite->setAnchorPoint(Point(0,0));
    batchNode->addChild(main_sprite);
    
    return true;
}

void NonPlayer::update(float dt){

    facePlayer(mainChar);
    if(auto_speech != ""){
        AutoConversation(auto_speech);
    }
    
    if(current_aa->getState() == States::DEFAULT && !current_aa->stateChange){
        
        if(isNearPlayer()){
            if(hasAction && !showAction){
                action_area->addInteraction(interaction_name);
                showAction = true;
            }
        
            if( action_area->click()){

                if(!convo->isConvoStarted() ){
                    current_scene->removeChild(action_area);//remove joystick first
                    convo = new Conversation();
                    convo->init(current_scene, convofile_url, char_name, 0, 0);
                    
                }
            }
        
        }else if(!isNearPlayer()){
            if(hasAction && showAction){
                action_area->removeInteraction(interaction_name);
                showAction = false;
            }
        }
    }else if(current_aa->getState() == States::DEFAULT && current_aa->stateChange){
        current_scene->removeChild(current_aa);
        current_aa = action_area;
        current_aa->setZOrder(0);
        current_scene->addChild(action_area);
        current_aa->stateChange = false;
    }
    
    
    if(convo != nullptr){
        if(convo->isConvoEnded() && convo->isWaitingReply()){
            //convo_started = false;

            //inflate speech Action Area and gets reply
            
            current_aa = new SpeechAA();
            printf("changed AA\n");
            current_aa->init();
            current_aa->LoadOptionLines(optionfile_url);
            current_aa->setupMenu(0,0, reply);
            current_scene->addChild(current_aa);

            current_tree_level++;
            convo->cleanup();
            //gives reply back to convo
            //TODO: check, condition feels dodgy
            
            //restart convo
            //convo->cleanup();
            //convo = nullptr;
            //convo = new Conversation();
            //convo->init(current_scene, convofile_url, current_tree_level, reply);
            //convo->setConvoEnded(false);

        }else if(convo->isConvoEnded() && !convo->isWaitingReply()){
            current_aa->setState(States::DEFAULT);
            printf("convo ended. no reply. cleanup\n");
            current_tree_level = 0;
            convo->cleanup();
            convo = nullptr;
            convo = new Conversation();
        }
        
        if(reply != -1){
            printf("gotten reply value : %d\n", reply);
            current_scene->removeChild(action_area);//remove joystick first
            convo->cleanup();
            convo = nullptr;
            convo = new Conversation();
            convo->init(current_scene, convofile_url, char_name,  current_tree_level, reply);
            reply = -1;
        }
    }
    
}

void NonPlayer::setPlayerSpeaks(bool speaks){     PlayerSpeaks = speaks; }
void NonPlayer::setActionArea(ActionArea* AA){    action_area = AA; }
void NonPlayer::setCurrentAA(ActionArea* AA){     current_aa = AA; }

void NonPlayer::setHasInteraction(bool yesno, const char* interaction_name){
    
    this->interaction_name = interaction_name;
    hasAction = yesno;
};

void NonPlayer::talk(std::string speech){
    /** this is how you animate a speech **/
    sentence = AnimatedLabel::create(speech, "visitor2.ttf", 12*4);
    sentence->setScale(0.25);
    layer_map->addChild(sentence);
    setSpeechPosition(sentence);
    
    sentence->AnimateTypewrite(0.5);
    DelayTime* nxt_sentence_delay = DelayTime::create(3);
    sentence->runAction(nxt_sentence_delay);

    AutoSpoken = true;

}


void NonPlayer::AutoConversation(std::string speech){
    if(isNearPlayer()){
        if(!AutoSpoken){
            talk(speech);
        }
        
//        NOTE: commented out. Not sure if there's any use for this, YET
//        if(sentence->isVisible() != false){
//            if(sentence->getNumberOfRunningActions() == 0){
//                sentence->setVisible(false);
//            }
//        }
        
    }else{   //release speech if not near player
        layer_map->removeChild(sentence, true);
        AutoSpoken = false;
    }
}

void NonPlayer::setSpeechPosition(AnimatedLabel* sentence){
    Point npc_position = main_sprite->getPosition();
    Point left_position = Point(npc_position.x + (main_sprite->getContentSize().width*main_sprite->getScale()) + 20,
                                npc_position.y + (main_sprite->getContentSize().height*main_sprite->getScale()) + 10);
    
    Point right_position = Point(npc_position.x + (main_sprite->getContentSize().width*main_sprite->getScale()) - 20,
                                npc_position.y + (main_sprite->getContentSize().height*main_sprite->getScale()) + 10);
    
    if(direction == LEFT){
        sentence->setPosition(left_position);
    }else{
        
        sentence->setPosition(right_position);
    }
}

void NonPlayer::setConvoFileurl(const char* url){
    convofile_url = url;
}

void NonPlayer::setAutoSpeech(std::string speech){
    auto_speech = speech;
}

bool NonPlayer::isNearPlayer(){
    Rect npc_box = main_sprite->getBoundingBox();
    Rect main_box = mainChar->main_sprite->getBoundingBox();
    
    Rect detect_area = Rect(npc_box.getMidX() - 100, npc_box.getMidY() - 100, npc_box.size.width + 130, npc_box.size.height + 100);
    
    if(detect_area.intersectsRect(main_box)){
        return NearPlayer = true;
    }
    return NearPlayer = false;
}

void NonPlayer::facePlayer(Character* facingWho){
    if(checkWhichSideisOn(facingWho)){
        //returns true if on the right side
        if(this->getDirection() != RIGHT){
            this->flipDirection();
        }
    }else{
        if(this->getDirection() != LEFT) {
            this->flipDirection();
        }
    }
}

bool NonPlayer::checkWhichSideisOn(Character* mainC){
    //check if its on right side of the NPC
    if(mainC->getCoordX() > this->getCoordX()){ //on the right side
        return true;
    }
    
    return false;
}


void NonPlayer::setSpawnPoint(float x, float y){    main_sprite->setPosition(Point(x,y)); }

void NonPlayer::setBubbleSprite(std::string filename){
    bubbleSprite->Sprite::create(filename);
}

void NonPlayer::setSpeechBubbleAnimation(const char* sprite_batch,
                                         const char* speech_plist, int nof_frames){
    if(bubbleSprite == NULL){
        CCLOG("Sprite is empty! please set up the sprite before animating it");
    }
    
    // "caches" are always singletons in cocos2d
    auto cache = AnimationCache::getInstance();
    cache->addAnimationsWithFile(speech_plist);
    // should be getAnimationByName(..) in future versions
    BubbleAnimation = cache->getAnimation("speechbubble");
    AnimateBubble = Animate::create(BubbleAnimation);
    bubbleSprite->runAction(AnimateBubble);
}

