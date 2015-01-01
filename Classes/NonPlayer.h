//
//  NonPlayer.h
//  battleout
//
//  Created by Adam on 7/14/14.
//
//

#ifndef __battleout__NonPlayer__
#define __battleout__NonPlayer__

#include <iostream>
#include "cocos2d.h"
#include "Character.h"
#include "AnimatedLabel.h"
#include "ActionArea.h"
#include "SpeechAA.h"
#include "Conversation.h"

using namespace cocos2d;

class NonPlayer : public Character{
public:
    bool init(const char* sprite_filename, const char* convo_filename, float x, float y, Layer* scene_layer, Character* mainC, Sprite* map);
    bool init(const char* sprite_filename, const char* convo_filename, const char* options_filename, float x, float y,
                         Layer* scene_layer, Character* mainC, Sprite* map);
    void update(float dt);
    
    void AutoConversation(std::string speech);
    void setAutoSpeech(std::string speech);
    
    void setPlayerSpeaks(bool speaks);
    void setSpeechBubbleAnimation(const char* sprite_batch, const char* speech_plist, int nof_frames);
    void setBubbleSprite(std::string filename);
    void setCurrentAA(ActionArea* AA);
    void setActionArea(ActionArea* AA);
    void setHasInteraction(bool yesno, const char* interaction_name);
    void setConvoFileurl(const char* url);
    
    void setSpawnPoint(float x, float y);
    Point getSpawnPoint();
    
    bool NearPlayer;
    AnimatedLabel* sentence;
    
protected:
    Sprite* speechSprite;
    Point spawnPoint;
    
private:
    Layer* current_scene;
    SpeechAA* pSpeechAA;
    ActionArea* action_area;
    ActionArea* current_aa;
    Sprite* layer_map;
    Sprite* bubbleSprite;
    Character* mainChar;
    Conversation* convo = new Conversation;
    const char* convofile_url;
    const char* replyfile_url;
    const char* optionfile_url;
    std::string auto_speech;
    std::string interaction_name;
    
    //state bools
    bool convo_started = false;
    bool AutoSpoken;
    bool showAction = false;
    bool hasAction;
    bool isNearPlayer();
    bool waiting_reply;
    bool PlayerSpeaks;
    
    Animation* BubbleAnimation;
    Animate* AnimateBubble;
    RepeatForever* speech_bubble;
    
    void facePlayer(Character* facingWho);
    bool checkWhichSideisOn(Character* mainC);
    void setSpeechPosition(AnimatedLabel* sentence);
    void talk(std::string speech);
    int current_tree_level = 0;
    int reply = -1;
    
};
#endif /* defined(__battleout__NonPlayer__) */
