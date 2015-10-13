//
//  MultiplePulsingHaloLayer.cpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/13/15.
//
//

#include "MultiplePulsingHaloLayer.hpp"
#include "PulsingHaloLayer.hpp"

MultiplePulsingHaloLayer* MultiplePulsingHaloLayer::create(const cocos2d::Color4B &color, float radius, int repeatCount) {
    MultiplePulsingHaloLayer *pRet = new(std::nothrow) MultiplePulsingHaloLayer();
    va_list vl;
    if (pRet && pRet->initWithColor(color, radius, repeatCount)) {
        pRet->autorelease();
    } else {
        CC_SAFE_DELETE(pRet);
    }
    
    va_end(vl);
    
    return pRet;
}

bool MultiplePulsingHaloLayer::initWithColor(const cocos2d::Color4B &color, float radius, int repeatCount) {
    if (Node::init()) {
        _haloLayerNumber = 2;
        _fromValueForRadius = 0.0;
        _fromValueForAlpha = 0.45;
        _keyTimeForHalfOpacity = 0.7f;
        _animationDuration = 3.f;
        _animationRepeatCount = 0;
        _startInterval = 1;
        _haloLayerColor = color;
        _radius = radius;
        
        return true;
    }
    
    return false;
}

void MultiplePulsingHaloLayer::buildSubLayers() {
    for (int i=0; i< _haloLayerNumber; i++) {
        PulsingHaloLayer *layer = PulsingHaloLayer::create(_haloLayerColor, _radius, _animationRepeatCount);
        layer->setAnimationDuration(_animationDuration);
        layer->setFromValueForAlpha(_fromValueForAlpha);
        layer->setFromValueForRadius(_fromValueForRadius);
        layer->setKeyTimeForHalfOpacity(_keyTimeForHalfOpacity);
        layer->setDelayInterval(_startInterval * i);
        
        layer->reset();
        
        this->addChild(layer);
    }
}

void MultiplePulsingHaloLayer::start() {
    std::string name = "PulsingHaloLayer";
    
    for (int i=0; i<this->getChildren().size(); i++) {
        auto obj = this->getChildren().at(i);
        if (obj->getName() == name) {
            PulsingHaloLayer *layer = static_cast<PulsingHaloLayer*>(obj);
            layer->start();
        }
    }
}