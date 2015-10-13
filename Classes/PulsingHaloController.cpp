//
//  PulsingHaloController.cpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/13/15.
//
//

#include "PulsingHaloController.hpp"
USING_NS_CC;

#define kPulsingHaloLayerActionTag 2000

PulsingHaloController* PulsingHaloController::create() {
    PulsingHaloController * pRet = new (std::nothrow) PulsingHaloController();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

PulsingHaloController::PulsingHaloController() {
    
}

PulsingHaloController::~PulsingHaloController() {
    
}

bool PulsingHaloController::init() {
    _fromValueForRadius = 0.0;
    _fromValueForAlpha = 0.45;
    _keyTimeForHalfOpacity = 0.7;
    _animationDuration = 3;
    _repeatCount = 0;
    
    return true;
}

void PulsingHaloController::onAdd() {
    _owner->setScale(_fromValueForRadius);
    _owner->setOpacity(_fromValueForAlpha * 255);
}

void PulsingHaloController::onRemove() {
    _owner->setScale(_fromValueForRadius);
    _owner->setOpacity(_fromValueForAlpha * 255);
}

void PulsingHaloController::update(float delta) {
}

void PulsingHaloController::start() {
    ScaleTo *scaleEffect = ScaleTo::create(_animationDuration, 1.0);
    
    FadeTo *fadeToEffect = FadeTo::create(_animationDuration * _keyTimeForHalfOpacity, 0.45);
    FadeOut *fadeOutEffect = FadeOut::create(_animationDuration * (1 - _keyTimeForHalfOpacity));
    ActionInterval *fadeEffect = Sequence::createWithTwoActions(fadeToEffect, fadeOutEffect);
    
    CallFunc *reset = CallFunc::create([&] () {
        _owner->setScale(_fromValueForRadius);
        _owner->setOpacity(_fromValueForAlpha * 255);
    });
    
    ActionInterval *mainAction = Sequence::create(Spawn::createWithTwoActions(scaleEffect, fadeEffect), reset, NULL) ;
    
    Action *action = NULL;
    if (_repeatCount == 1) {
        action = Sequence::create(mainAction, CallFunc::create([&] () {
            _owner->stopActionByTag(kPulsingHaloLayerActionTag);
        }), NULL);
    } else if (_repeatCount > 1 && _repeatCount < INFINITY) {
        action = Sequence::create(Repeat::create(mainAction, _repeatCount), CallFunc::create([&] () {
            _owner->stopActionByTag(kPulsingHaloLayerActionTag);
        }), NULL);
    } else {
        action = RepeatForever::create(mainAction);
    }
    
    action->setTag(kPulsingHaloLayerActionTag);
    
    _owner->runAction(action);
}