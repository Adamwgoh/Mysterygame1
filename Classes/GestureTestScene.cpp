//
//  GestureTestScene.cpp
//  battleout
//
//  Created by Adam on 9/5/14.
//
//

#include "GestureTestScene.h"
#include "GeometricTypes.h"

Scene* GestureTestScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    // 'layer' is an autorelease object
    auto layer = GestureTestScene::create();
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

bool GestureTestScene::init()
{
    if(!LayerColor::init()){
        
        return false;
    }
    initWithColor(Color4B::WHITE);
    
    translator = new DollarRecognizer::TemplateRecognizer();
    translator->loadTemplates();
    this->setAnchorPoint(Point(0,0));
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(GestureTestScene::onTouchBegan,this);
    listener->onTouchMoved = CC_CALLBACK_2(GestureTestScene::onTouchMoved,this);
    listener->onTouchEnded = CC_CALLBACK_2(GestureTestScene::onTouchEnded,this);
    
    dispatcher->addEventListenerWithFixedPriority(listener, 3);
    
    this->scheduleUpdate();
    
    return true;
}


void GestureTestScene::update(float dt){

}

bool GestureTestScene::onTouchBegan(Touch* touch, Event* event){
    //gesture = new std::vector<Touch*>();
    gesture = new DollarRecognizer::Path2D();
    DollarRecognizer::Point2D point((int)touch->getLocationInView().x,(int)touch->getLocationInView().y);
//    DollarRecognizer::Point2D point((int)touch->getLocation().x,(int)touch->getLocation().y);
    printf("point x is : %f, y is : %f\n", point.x, point.y);
    paint = SpriteBatchNode::create("joy_center.png");
    auto spot = Sprite::createWithTexture(paint->getTexture());
    spot->setPosition(touch->getLocationInView().x, touch->getLocationInView().y);
    this->addChild(spot);
    gesture->push_back(point);
    
    return true;
}


void GestureTestScene::onTouchMoved(Touch* touch, Event* event){
    if(gesture != NULL){
        paint = SpriteBatchNode::create("joy_center.png");
        auto spot = Sprite::createWithTexture(paint->getTexture());
        spot->setPosition(touch->getLocationInView().x, touch->getLocationInView().y);
        this->addChild(spot);

        DollarRecognizer::Point2D point((int)touch->getLocationInView().x,(int)touch->getLocationInView().y);
//        DollarRecognizer::Point2D point((int)touch->getLocation().x,(int)touch->getLocation().y);
        printf("point x is : %f, y is : %f\n", point.x, point.y);
        gesture->push_back(point);
    }
}

void GestureTestScene::onTouchEnded(Touch* touch, Event* event){
    this->removeAllChildrenWithCleanup(true);
    if(gesture != nullptr && gesture->size() > 1){
        //recognize here
        //DollarRecognizer::scaletoSquare(*gesture);
        DollarRecognizer::RecognitionResult result = translator->recognize(*gesture);
        
        printf("detected pattern: %s, score : %f\n", result.name.c_str(), result.score);

        //clear variable
        gesture->clear();
    }else{
        printf("gesture is null");
    }
}