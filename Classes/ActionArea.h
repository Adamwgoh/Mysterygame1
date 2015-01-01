//
//  ActionArea.h
//  battleout
//
//  Created by Adam on 7/3/14.
//
//

#ifndef __battleout__ActionArea__
#define __battleout__ActionArea__

#include "cocos2d.h"
#include "joystick.h"

using namespace cocos2d;

namespace States{
    
    enum State{ DEFAULT, SPEECHAA };
}

class ActionArea : public Layer{
public:


    static ActionArea* createInstance();
	bool init();
    void setParentScene(Layer* scene);
    void addInteraction(std::string action);
    void removeInteraction(std::string interaction);
    States::State getState();
    void setState(States::State state);
    
    virtual bool click();
    virtual bool LoadOptionLines(const char* replyfiles_url);
    virtual void setupMenu(int current_tree_level, int current_choice, int &replyy);
    virtual bool getOptionsStrings(int tree_level);
    virtual int  getReply();
    joystick* getJoystick();
    Sprite* getAAbox();
    bool stateChange = false;
    
    
	CREATE_FUNC(ActionArea);
    Sprite* box;

private:
    Layer* current_scene;
    States::State the_state;
    joystick* js;
    Label* action_name;
};

#endif /* defined(__battleout__ActionArea__) */
