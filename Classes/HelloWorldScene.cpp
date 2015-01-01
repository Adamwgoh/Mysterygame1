#include "HelloWorldScene.h"
#include "ActionArea.h"
#include "SpeechAA.h"
#include "AnimatedLabel.h"
#include "PauseLayer.h"
#include "VisibleRect.h"

USING_NS_CC;
#define PTM_RATIO 32

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();
    // add layer as a child to scene
    scene->addChild(layer);
    // return the scene
    return scene;
}

void HelloWorld::callPause(Object* pSender){
    //create new Pause Layer
    if(!scene_paused && this->getChildByTag(555) == NULL){
        printf("click\n");
        PauseLayer* pause_layer = new PauseLayer();
        pause_layer->inflate();
        pause_layer->setParentScene(this);
        this->addChild(pause_layer);
        this->setZOrder(3);
        pause_layer->setTag(555);
        scene_paused = true;
    }
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if(!Layer::init()){
        
        return false;
    }
    /**
    b2Vec2 gravity;
    gravity.Set(0.0f,0.0f);
    _world = new b2World(gravity);
    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    b2BodyDef ground;
    b2Body* groundBody = _world->CreateBody(&ground);
    ground.position.Set(0,0);
    b2EdgeShape groundBox;

    // bottom
    groundBox.Set(b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    
    // top
    groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    
    // left
    groundBox.Set(b2Vec2(VisibleRect::leftTop().x/PTM_RATIO,VisibleRect::leftTop().y/PTM_RATIO), b2Vec2(VisibleRect::leftBottom().x/PTM_RATIO,VisibleRect::leftBottom().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    
    // right
    groundBox.Set(b2Vec2(VisibleRect::rightBottom().x/PTM_RATIO,VisibleRect::rightBottom().y/PTM_RATIO), b2Vec2(VisibleRect::rightTop().x/PTM_RATIO,VisibleRect::rightTop().y/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);
    **/
    
    //adds actionArea onto scene
    this->setAnchorPoint(Point(0,0));
    action_area = new ActionArea();
    action_area->init();
    action_area->setParentScene(this);
    action_area->setZOrder(1);
    this->addChild(action_area);

    //loads map
    new_map = Sprite::create("background3.png");
    new_map->setAnchorPoint(Point(0,0));
    new_map->setPosition(Point(0,30));
    new_map->setZOrder(0);
    new_map->setScale(2.0, 2.0);
    
    
    this->addChild(new_map);

    //loads player
    Size winSize = Director::getInstance()->getWinSize();
    torc = new Character();
    Point position = new_map->convertToNodeSpaceAR(Point(100, 470));
    torc->init("character1.png", position.x, position.y, this);
    torc->getMainSprite()->setScale(.2f, .2f);
    if(torc->getBatchNode() == NULL){
        printf("l is null!\n");
    }
    
    new_map->addChild(torc->getBatchNode());
    
    //loads NPCs
    npc = new NonPlayer();
    Point npcposition = new_map->convertToNodeSpaceAR(Point(100, 470));
    printf("%d\n",FileUtils::getInstance()->isFileExist("ConvoLines.txt"));
    
    npc->init("character1.png", "ConvoLines.txt","OptionLines.txt",
              npcposition.x, npcposition.y, this, torc, new_map);
    
    //bunch of initialisations
    npc->setAutoSpeech("What you looking at?");
    npc->setCharacterName("macha");
    npc->setActionArea(this->action_area);
    npc->setCurrentAA(this->action_area);
    npc->setHasInteraction(true, "Interact");
    npc->setPlayerSpeaks(false);
    npc->getMainSprite()->setScale(.2f, .2f);
    if(npc->getBatchNode() == NULL){
        printf("npc batchnode is null");
    }

    new_map->addChild(npc->getBatchNode());
    
    //map touch listeners
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
    listener->onTouchCancelled = CC_CALLBACK_2(HelloWorld::onTouchCancelled, this);
    
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    //listener->setSwallowTouches(true);

    //dispatcher->addEventListenerWithSceneGraphPriority(listener, new_map);
    
    //multiple touch listener
    auto multi_listener = EventListenerTouchAllAtOnce::create();
    multi_listener->onTouchesBegan =     CC_CALLBACK_2(HelloWorld::onTouchesBegan, this);
    multi_listener->onTouchesMoved =     CC_CALLBACK_2(HelloWorld::onTouchesMoved, this);
    multi_listener->onTouchesEnded =     CC_CALLBACK_2(HelloWorld::onTouchesEnded, this);
    multi_listener->onTouchesCancelled = CC_CALLBACK_2(HelloWorld::onTouchesCancelled, this);
    multi_listener->setEnabled(true);

    dispatcher->addEventListenerWithSceneGraphPriority(multi_listener, new_map);
    
    //initialises touches holder
    _touches = new Vector<Touch*>();
    
    //set update method loop and done!
    this->scheduleUpdate();
    current_playview = new_map->getPosition();
    
    //pause button
    Label* pause_button = Label::createWithTTF("PAUSE", "Pxlvetica.ttf", 30);
    MenuItem* pause = MenuItemLabel::create(pause_button, CC_CALLBACK_1(HelloWorld::callPause, this));
    Menu* pauseMenu = Menu::create(pause, NULL);
    pauseMenu->setPosition(500,900);
    pauseMenu->setZOrder(1);
    this->addChild(pauseMenu);
    
    
    return true;
    
}

bool HelloWorld::checkScrolling(){
    if(mapScroll)   return false;
    return true;
}

void HelloWorld::update(float dt){
    if(!scene_paused && this->getChildByTag(555) != nullptr){
        this->removeChildByTag(555);
    }else if(scene_paused && this->getChildByTag(555) == nullptr){  //bug quick fix, where cant pause the second time onwards
        scene_paused = false;
    }
    
    if(checkScrolling()){
        
        moveCamera();
        npc->update(dt);
    }else{
        
    }

}

void HelloWorld::moveCamera(){
    Point velocity = action_area->getJoystick()->getVelocity();
    Point real_char_pos = new_map->convertToNodeSpace(torc->getPosition());
    Point real_char = torc->getPosition();

    resetView();
    //printf("anddddd guess what : %f\n", real_char_pos.x/(new_map->getContentSize().width*new_map->getScale()));
    //from right turning to left
    if(torc->getDirection() == 5){ //&& real_char_pos.x/(new_map->getContentSize().width*new_map->getScale()) < 0.95){

        setPerspective(5);
    }else if(torc->getDirection() == 4){// && real_char_pos.x/(new_map->getContentSize().width*new_map->getScale()) > 0.1){

        setPerspective(4);
    }

    torc->move(velocity*torc->getMainSprite()->getScale(), new_map->getContentSize());
    if(new_map->getScale() > 1){
        moveMap(torc->getMainSprite()->getScale()*velocity*20, torc->getDirection());
    }else{
        moveMap(torc->getMainSprite()->getScale()*velocity*10, torc->getDirection());
    }
    current_playview = new_map->getPosition();
}


void HelloWorld::moveMap(Point speed, int direction){

    Size winSize = Director::getInstance()->getWinSize();
    Size map_size = new_map->getContentSize();
    Point current_position = new_map->convertToNodeSpace(torc->getPosition());
    Point new_position = Point(new_map->getPosition().x - (speed.x),new_map->getPosition().y);

    if(new_position.x > 0)  new_position.x = 0;
    if(new_position.x < ( (-new_map->getContentSize().width*new_map->getScale()) + Director::getInstance()->getWinSize().width ))
        new_position.x =
            ( (-new_map->getContentSize().width*new_map->getScale()) + Director::getInstance()->getWinSize().width );
 
    //printf("moveMap : %f\n", (torc->getPosition().x/map_size.width * new_map->getScale()));
    
    //if at the last 10% of the map, do not move
    if( (current_position.x/map_size.width * new_map->getScale()) * new_map->getScale() < 0.1 ||
        (torc->getPosition().x/map_size.width * new_map->getScale()) > 0.97*new_map->getScale() ){
        //printf("holdd\n");
        return;
    }else{

        new_map->setPosition(new_position);
    }
}

bool HelloWorld::setPerspective(int direction){
    //going right is -1, going left is +1
    Size winSize = Director::getInstance()->getWinSizeInPixels();
    
    Point end_map = new_map->convertToNodeSpace(Point(winSize.width,0));
    Point map_pos = new_map->convertToNodeSpace(Point(0,0));
    //Point real_char_pos = new_map->convertToNodeSpace(torc->getPosition());
    Point real_char_pos = torc->getPosition();

    float left_space  = real_char_pos.x - map_pos.x;
    float right_space = end_map.x - real_char_pos.x;
    

    //4 is left, 5 is right
    if(direction == 5){
        //check which side has more, if left had more, swap
        if((left_space > right_space && left_space != right_space) || left_space/right_space > 0.333){
            
            moveMap(Point(+17, 0), 5);
            
            return true;
        }
        
    }else if(direction == 4){
        //same thing happens here as well
        if((right_space  > left_space && right_space != left_space) || right_space/left_space > 0.333){
            
            moveMap(Point((-17), 0), 4);
            
            return true;
       }
        
    }
    
    return false;
}


bool HelloWorld::onTouchBegan(Touch* touch, Event* event){

    if(scene_paused){
        return false;
    }
    //get touch location
    Point location = touch->getLocation();

    Rect act_box = action_area->box->getBoundingBox();
    if(act_box.containsPoint(touch->getLocation())){
        mapScroll = false;
        
        return false;
    }
    return true;
}

void HelloWorld::onTouchMoved(Touch* touch, Event* event){
        //write new method to put this, put boundaries, set its z value to below action area
        Point prev_location = touch->getPreviousLocation();
        Point current_location = touch->getLocation();
        mapScroll = true;
    printf("scrolling\n");
        auto dx = current_location.x - prev_location.x;
        auto dy = current_location.y - prev_location.y;
        ScrollMap(dx, dy);
        

}

void HelloWorld::onTouchEnded(Touch* touch, Event* event){
  
}

void HelloWorld::onTouchCancelled(Touch* touch, Event* event){
  
}

//DONE: not working. this is called when AA is touched instead. whattheheck

void HelloWorld::onTouchesBegan(std::vector<Touch*> touches, Event* event){

    if(!touches.empty()){
        for(int i = 0; i < touches.size(); i++){

            _touches->pushBack(touches.at(i));
        }
    }
    
    
    
//    if(_touches->size() ==1){
//        
//        Touch* first_touch = _touches->at(0);
//        onTouchBegan(first_touch, event);
//    }
    
}

//TODO: there is a tendency to activate onTouchMove when there is only one finger moving(aka one touches size)
void HelloWorld::onTouchesMoved(std::vector<Touch*> touches, Event* event){

    
    //printf("stored touches size : %lu\n", _touches->size());
    //printf("Moving touches size : %lu\n", touches.size());
    
    if(_touches->size() == 1){

        Touch* first_touch = _touches->at(0);
        if(onTouchBegan(first_touch, event)){

            onTouchMoved(first_touch, event);
        }
        
    }else if(_touches->size() >= 2){
        if(mapScroll) mapScroll = false;
        //get the two touches
        Touch* touch_1 = _touches->at(0);
        Touch* touch_2 = _touches->at(1);
        
        //get current and previous locations of both touches
        Point curPosPoint1  = Director::getInstance()->convertToGL(touch_1->getLocationInView());
        Point prevPosPoint1 = Director::getInstance()->convertToGL(touch_1->getPreviousLocationInView());
        Point curPosPoint2  = Director::getInstance()->convertToGL(touch_2->getLocationInView());
        Point prevPosPoint2 = Director::getInstance()->convertToGL(touch_2->getPreviousLocationInView());
        
        //get the midpoint between two touches
        Point curPosLayer = curPosPoint1.getMidpoint(curPosPoint2);
        Point prevPosLayer = prevPosPoint1.getMidpoint(prevPosPoint2);
        
        
        
        //calculate new scale
        float prev_scale = new_map->getScale();
        float current_scale = new_map->getScale() * (curPosPoint1.getDistance(curPosPoint2)/ prevPosPoint1.getDistance(prevPosPoint2));
    
        new_map->setScale( MIN( MAX( current_scale, 1.0 ), 2.0 ) );
        
        

        if(this->getScale() != prev_scale){
            Point current_posLayer = new_map->convertToNodeSpaceAR(curPosLayer);
            float delta_x = current_posLayer.x * (new_map->getScale() - prev_scale);
            float delta_y = current_posLayer.y * (new_map->getScale() - prev_scale);

            //printf("doing this\n");
            setBoundedPosition(Point(new_map->getPositionX() - delta_x, new_map->getPositionY() + delta_y), new_map);
            
        }
        
        // If current and previous position of the multitouch's center aren't equal -> change position of the layer
		if (!prevPosLayer.equals(curPosLayer))
		{
            //printf("doing that\n");
            //if you want to zoom out to where, this is the place to change
            Point current_posLayer = new_map->convertToNodeSpaceAR(curPosLayer);
            setBoundedPosition(Point(new_map->getPosition().x + curPosLayer.x - prevPosLayer.x,
                                  new_map->getPosition().y + curPosLayer.y + prevPosLayer.y), new_map);
		}
    }
    
    
    
}
//TODO: left right and top boundaries are still wrong
void HelloWorld::setBoundedPosition(Point position, Node* obj){

    new_map->setPosition(position);
   
    Rect _panBoundsRect = Rect(
                               obj->getContentSize().width *-0.5,
                               obj->getContentSize().height *-0.5,
                               obj->getContentSize().width * 1,
                               (obj->getContentSize().height * 1)-232 );
    
    if( _panBoundsRect.equals( Rect::ZERO ) == false )
	{
		Rect boundBox;
		boundBox.origin = this->getPosition() / this->getScale();
		boundBox.size = this->getContentSize() / this->getScale();

        
		// OpenGL coordinate system
		float left = boundBox.getMinX();
		float right = boundBox.getMaxX();
		float top = boundBox.getMaxY();
		float bottom = boundBox.getMinY();
        
		float min_x = _panBoundsRect.getMinX() + boundBox.size.width;
		float max_x = _panBoundsRect.getMaxX() + boundBox.size.width;
		float min_y = _panBoundsRect.getMinY() - boundBox.size.height;
		float max_y = _panBoundsRect.getMaxY() + boundBox.size.height;
        
        
		float scale = obj->getScale();
		float arLeft = min_x;
		float arRight = (max_x) - this->getContentSize().width;
		float arTop = max_y * scale - this->getContentSize().height;
        float arBottom = (min_y * scale);
        //printf("left : %f, min_x : %f", left, min_x);
		//printf("right : %f, max_x : %f", right, max_x);
        
		if( left < min_x )
		{
            //printf("left boudned\n");
			obj->setPosition( arLeft, new_map->getPosition().y );
		}
        
		if( right > max_x )
		{
            
            //printf("right boudned\n");
			obj->setPosition( arRight, new_map->getPosition().y );
		}
        
		if( top > max_y )
		{

			obj->setPosition( new_map->getPosition().x, arTop );
		}
        
        
        if( bottom < min_y )
        {
            //printf("bottom boudned\n");
            obj->setPosition( new_map->getPosition().x, arBottom );
        }
        
	}
}

void HelloWorld::onTouchesEnded(std::vector<Touch*> touches, Event* event){
    
    if(touches.size() == _touches->size())  _touches->clear();
    for(int i = 0; i < touches.size(); i++){
        _touches->eraseObject(touches.at(i));
    }
    
    
    if(new_map->getScale() != 1 || new_map->getScale() != 2){
        if(2 - new_map->getScale() < 0.6 ){
            //set scale to 2
            auto scale = ScaleTo::create(new_map->getScale(), 2);
            scale->setDuration(0.5);
            new_map->runAction(scale);
        }else{
            //set scale to 1
            auto scale = ScaleTo::create(new_map->getScale(), 1);
            scale->setDuration(0.5);
            new_map->runAction(scale);
        }
    }
}

void HelloWorld::onTouchesCancelled(std::vector<Touch*> touches, Event* event){
    printf("canceled\n");
}

void HelloWorld::ScrollMap(float dx, float dy){
    
    if(new_map->getPositionX() + dx < 0 &&
       (new_map->getPositionX() + dx >  ( (-new_map->getContentSize().width*new_map->getScale()) + Director::getInstance()->getWinSize().width )) ){
        
        new_map->setPositionX(new_map->getPositionX() + dx);
        
    }else if(new_map->getPositionY() + dy > (new_map->getContentSize().height-904)  &&
       (new_map->getPositionY() + dy) < (-new_map->getContentSize().height+904)){
       
        new_map->setPositionY(new_map->getPositionY() + dy);
    }
}

bool HelloWorld::getScenePause(){
    return scene_paused;
}

void HelloWorld::setScenePause(bool paused){
    scene_paused = paused;
}

//DONE: reset view
void HelloWorld::resetView(){
    new_map->setPosition(current_playview);
}
