//
//  Character.cpp
//  battleout
//  This class is a generic class for all sprites character including
//  the main character, NPCs, and etcs. This is not a class to be extended
//  upon for interactable objects
//  Created by Adam on 7/9/14.
//
//

#include "Character.h"

#define RUNNING_SPEED 10
#define IDLE 1
#define MOVE 2
#define INTERACT 3
#define LEFT 4
#define RIGHT 5


bool Character::init(const char* sprite_filename, float x, float y, Layer* scene){
//    main_sprite = Sprite::create(sprite_filename);
//DONE: setup sprites and position
    batchNode = SpriteBatchNode::create(sprite_filename);
    scene_layer = scene;
    
    main_sprite = Sprite::create(sprite_filename);
    main_sprite->setAnchorPoint(Point(0,0));
    main_sprite->setPosition(Point(x, y));
    batchNode->addChild(main_sprite);

    
    return true;

}

void Character::setIdleAnimation(const char* sprite_batch, const char* idle_plist, int nof_frames){
    batchNode = SpriteBatchNode::create(sprite_batch);
    SpriteFrameCache* framecache = SpriteFrameCache::getInstance();
    framecache->addSpriteFramesWithFile(sprite_batch);
    
    //DONE: setup idle animation movement
    //get a modified sprite file name without extension
    std::string str(idle_plist, strnlen(0, strlen(idle_plist)- 5));//minus .plist
    Vector<SpriteFrame*> idle_aniframes(nof_frames);
    
    for(int i =0; i< nof_frames; i++){
        str = str + std::to_string(i) + ".png";
        printf("just checking, str is %s", str.c_str());
        SpriteFrame* frame = framecache->getSpriteFrameByName(str);
        idle_aniframes.pushBack(frame);
        
    }
    
    animationDefault = Animation::createWithSpriteFrames(idle_aniframes);
    animateDefault = Animate::create(animationDefault);
    actionStateDefault = RepeatForever::create(animateDefault);
//    
//    // "caches" are always singletons in cocos2d
//    auto cache = AnimationCache::getInstance();
//    cache->addAnimationsWithFile(idle_plist);
//    // should be getAnimationByName(..) in future versions
//    animationDefault = cache->animationByName("default");
//    animateDefault = Animate::create(animationDefault);
//   // main_sprite->runAction(animate);
    
    
    //run action with
    //    Animate* idle = Animate::create(animationDefault);
    //    main_sprite->runAction(RepeatForever::create(idle));
    

}

void Character::setMovingAnimation(const char* sprite_batch, const char* idle_plist, int nof_frames){
    batchNode = SpriteBatchNode::create(sprite_batch);
    SpriteFrameCache* framecache = SpriteFrameCache::getInstance();
    framecache->addSpriteFramesWithFile(sprite_batch);
    
    //DONE: setup idle animation movement
    //get a modified sprite file name without extension
    std::string str(idle_plist, strnlen(0, strlen(idle_plist)- 5));//minus .plist
    Vector<SpriteFrame*> moving_aniframes(nof_frames);
    
    for(int i =0; i< nof_frames; i++){
        str = str + std::to_string(i) + ".png";
        printf("just checking, str is %s", str.c_str());
        SpriteFrame* frame = framecache->getSpriteFrameByName(str);
        moving_aniframes.pushBack(frame);
        
    }
    
    animationMoving = Animation::createWithSpriteFrames(moving_aniframes);
    animateMoving = Animate::create(animationMoving);
    actionStateMoving = RepeatForever::create(animateMoving);
}

void Character::flipDirection(){
    if(direction == LEFT){//assumes sprite is facing right at all times
        direction = RIGHT;  main_sprite->setFlippedX(false);
    }else if(direction == RIGHT){
        direction = LEFT;   main_sprite->setFlippedX(true);
    }//can add front and back later on
}

void Character::setDirection(int direction){
    if(direction != LEFT || direction != RIGHT){//can add front and back later on
        printf("error value! ");
    }
    this->direction = direction;
}


void Character::move(Point speed, Size map_size){
    //sprite face left and asked to move right
    if(speed.x > 0){
        if(direction == LEFT){
            
            flipDirection();
        }
        setState(MOVE);
        
    }else if(speed.x < 0){
        if(direction == RIGHT){
            
            flipDirection();
        }
        setState(MOVE);
        
    }else{
        
        //not moving
        setState(IDLE);
    }
    
    if(state != IDLE) {
        //TODO: see if you can refactor this to use moveBy and Sequence
        float new_x = main_sprite->getPosition().x + (speed.x * RUNNING_SPEED);
        float new_y = main_sprite->getPosition().y + speed.y;
        //printf("new_x is : %f, map_size is %f\n", new_x, map_size.width);

        //keep char in map
        if(new_x < 0)   new_x = 0;
        if(new_x > map_size.width - map_size.width*0.003)  new_x = map_size.width - map_size.width*0.003;
        //.03% because theres some wierd border prob
        
        main_sprite->setPosition( Point(new_x, main_sprite->getPositionY()) );
    }
    
}

int Character::getDirection(){    return direction; }
const char* Character::getCharacterName(){    return char_name; }
void Character::setCharacterName(const char* character){  this->char_name = character; }

void Character::setState(int state){    this->state = state; }
int Character::getState(){    return state; }

void Character::update(){
//TODO: leave for Box2d physics
    printf("char update method here \n");
}

Sprite* Character::getMainSprite(){
    return main_sprite;
}
void Character::setSpriteBatchNode(SpriteBatchNode* batchnode){     batchNode = batchnode; }
SpriteBatchNode* Character::getBatchNode(){    return batchNode; }

Point Character::getPosition(){    return main_sprite->getPosition(); }
float Character::getCoordX(){    return main_sprite->getPositionX(); }
float Character::getCoordY(){    return main_sprite->getPositionY(); }

void Character::setPosition(Point position){    main_sprite->setPosition(position); }
void Character::setCoordX(float val_x){    main_sprite->setPositionX(val_x); }
void Character::setCoordY(float val_y){    main_sprite->setPositionY(val_y); }

