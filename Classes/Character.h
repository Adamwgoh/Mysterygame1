//
//  Character.h
//  battleout
//  This class is a generic class for all sprites character including
//  the main character, NPCs, and etcs. This is not a class to be extended
//  upon for interactable objects
//  Created by Adam on 7/9/14.
//
//

#ifndef __battleout__Character__
#define __battleout__Character__

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class Character {
public:
    bool init(const char* sprite_filename, float x, float y, Layer* scene_layer);
    void setMovingAnimation(const char* sprite_batch, const char* moving_plist, int nof_frames);
    void setIdleAnimation(const char* sprite_batch, const char* idle_plist, int nof_frames);
    
    void setSpriteBatchNode(SpriteBatchNode* batchnode);
    SpriteBatchNode* getBatchNode();
    Sprite* getMainSprite();
    Sprite* main_sprite;
    
    int getDirection();
    void flipDirection();//opposite of the current direction
    void setDirection(int Direction);
    
    const char* getCharacterName();
    void setCharacterName(const char* character);
    
    void setState(int state);
    int  getState();
    
    Point getPosition();
    float getCoordX();
    float getCoordY();
    
    void setPosition(Point position);
    void setCoordX(float val_x);
    void setCoordY(float val_y);
    
    void move(Point velocity, Size map_size);
    
    

protected:
    Layer* scene_layer;
    const char* char_name;
    int state;
    int direction = 5;//5 as right
    
    RepeatForever *actionStateDefault;
    RepeatForever *actionStateMoving;
    Animate *animateDefault;
    Animate *animateMoving;
    Animation *animationDefault;
    Animation *animationMoving;
    SpriteBatchNode *batchNode;
    
private:
    void update();
    
    
    
};
#endif /* defined(__battleout__Character__) */
