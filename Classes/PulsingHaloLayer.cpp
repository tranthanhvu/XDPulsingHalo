//
//  PulsingHaloLayer.cpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/12/15.
//
//

#include "PulsingHaloLayer.hpp"

#define kPulsingHaloLayerActionTag 2000

PulsingHaloLayer* PulsingHaloLayer::create(const cocos2d::Color4B &color, float radius, int repeatCount) {
    PulsingHaloLayer *pRet = new(std::nothrow) PulsingHaloLayer();

    if (pRet && pRet->initWithColor(color, radius, repeatCount))
    {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

PulsingHaloLayer::PulsingHaloLayer() {
    
}

PulsingHaloLayer::~PulsingHaloLayer() {
    
}

bool PulsingHaloLayer::initWithColor(const cocos2d::Color4B &color, float radius, int repeatCount) {
    if (CircleLayer::initWithColor(color, radius)) {
        setName("PulsingHaloLayer");
        
        _fromValueForRadius = 0.0;
        _fromValueForAlpha = 0.45;
        _keyTimeForHalfOpacity = 0.7f;
        _animationDuration = 3.f;
        _delayInterval = 0;
        _repeatCount = repeatCount;
        
        this->reset();
        
        return true;
    }
    
    return false;
}

void PulsingHaloLayer::start() {
    
    ScaleTo *scaleEffect = ScaleTo::create(_animationDuration, 1.0);
    
    FadeTo *fadeToEffect = FadeTo::create(_animationDuration * _keyTimeForHalfOpacity, 0.45);
    FadeOut *fadeOutEffect = FadeOut::create(_animationDuration * (1.f - _keyTimeForHalfOpacity));
    ActionInterval *fadeEffect = Sequence::createWithTwoActions(fadeToEffect, fadeOutEffect);
    
    CallFunc *reset = CallFunc::create([&] () {
        this->reset();
    });
    
    ActionInterval *mainAction = NULL;
    if (_delayInterval <= 0 || _delayInterval == INFINITY) {
        mainAction = Sequence::create(Spawn::createWithTwoActions(scaleEffect, fadeEffect), reset, NULL);
    } else {
        // create a delay action
        mainAction = Sequence::create(DelayTime::create(_delayInterval), Spawn::createWithTwoActions(scaleEffect, fadeEffect), reset, NULL) ;
    }
    
    Action *action = NULL;
    if (_repeatCount == 1) {
        action = Sequence::create(mainAction, CallFunc::create([&] () {
            this->stopActionByTag(kPulsingHaloLayerActionTag);
        }), NULL);
    } else if (_repeatCount > 1 && _repeatCount < INFINITY) {
        action = Sequence::create(Repeat::create(mainAction, _repeatCount), CallFunc::create([&] () {
            this->stopActionByTag(kPulsingHaloLayerActionTag);
        }), NULL);
    } else {
        action = RepeatForever::create(mainAction);
    }
    
    action->setTag(kPulsingHaloLayerActionTag);
    
    this->runAction(action);
}

void PulsingHaloLayer::reset() {
    this->setScale(_fromValueForRadius);
    this->setOpacity(_fromValueForAlpha * 255);
}