#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "PulsingHaloLayer.hpp"
#include "CircleLayer.hpp"
#include "PulsingHaloController.hpp"

USING_NS_CC;

using namespace cocostudio::timeline;

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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto rootNode = CSLoader::createNode("MainScene.csb");

    addChild(rootNode);
    
    PulsingHaloEffect* halo = PulsingHaloEffect::create();
    halo->setPosition(rootNode->getContentSize() * 0.5);
    addChild(halo);
    halo->start();
    
    CircleLayer *circleLayer = CircleLayer::create(Color4B::BLUE, 300);
    circleLayer->setPosition(rootNode->getContentSize() * 0.2);
    addChild(circleLayer);
    
    PulsingHaloController* com = PulsingHaloController::create();
    circleLayer->addComponent(com);
    com->start();

    return true;
}
