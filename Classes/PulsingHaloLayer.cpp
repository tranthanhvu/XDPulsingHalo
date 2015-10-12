//
//  PulsingHaloLayer.cpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/12/15.
//
//

#include "PulsingHaloLayer.hpp"

#define kPulsingHaloLayerActionTag 2000

PulsingHaloEffect* PulsingHaloEffect::create(const cocos2d::Color4B &color, float repeatCount) {
    PulsingHaloEffect *pRet = new(std::nothrow) PulsingHaloEffect();

    if (pRet && pRet->initWithColor(color, repeatCount))
    {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

PulsingHaloEffect::PulsingHaloEffect() {
    
}

PulsingHaloEffect::~PulsingHaloEffect() {
    
}

bool PulsingHaloEffect::initWithColor(const cocos2d::Color4B &color, float repeatCount) {
    if (LayerColor::initWithColor(color)) {
        _radius = 600;
        _fromValueForRadius = 0.0;
        _fromValueForAlpha = 0.45;
        _keyTimeForHalfOpacity = 0.2;
        _animationDuration = 3;
        _pulseInterval = 0;
        _repeatCount = repeatCount;
        _useTimingFunction = true;
        
        prepairEffect();
        
        return true;
    }
    
    return false;
}

void PulsingHaloEffect::prepairEffect() {
    float diameter = _radius * 2;
    changeWidthAndHeight(diameter, diameter);
    
    this->setScale(_fromValueForRadius);
    this->setOpacity(_fromValueForAlpha * 255);
}

void PulsingHaloEffect::start() {
    ScaleTo *scaleEffect = ScaleTo::create(_animationDuration, 1.0);
    
    FadeTo *fadeToEffect = FadeTo::create(_animationDuration * 0.5, 0.45);
    FadeOut *fadeOutEffect = FadeOut::create(_animationDuration * 0.5);
    ActionInterval *fadeEffect = Sequence::createWithTwoActions(fadeToEffect, fadeOutEffect);
    
    CallFunc *reset = CallFunc::create([&] () {
        this->setScale(_fromValueForRadius);
        this->setOpacity(_fromValueForAlpha * 255);
    });
    
    ActionInterval *mainAction = Sequence::create(Spawn::createWithTwoActions(scaleEffect, fadeEffect), reset, NULL) ;
    
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

void PulsingHaloEffect::end(const Point point, const std::function<void(Node*)>& endFunc) {
    
}