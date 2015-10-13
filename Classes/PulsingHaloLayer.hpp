//
//  PulsingHaloLayer.hpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/12/15.
//
//

#ifndef PulsingHaloLayer_hpp
#define PulsingHaloLayer_hpp

#include <stdio.h>
#include "CircleLayer.hpp"
USING_NS_CC;

class PulsingHaloEffect: public CircleLayer {
    
public:
    PulsingHaloEffect();
    ~PulsingHaloEffect();
    
    static PulsingHaloEffect* create(const cocos2d::Color4B &color = Color4B(0, 122, 255, 255), float radius = 300, int repeatCount = 0);
    
    bool initWithColor(const cocos2d::Color4B &color, float radius = 300, int repeatCount = 0);
    void start();
    
    CC_SYNTHESIZE(float, _fromValueForRadius, FromValueForRadius);
    CC_SYNTHESIZE(float, _fromValueForAlpha, FromValueForAlpha);
    CC_SYNTHESIZE(float, _keyTimeForHalfOpacity, KeyTimeForHalfOpacity);
    CC_SYNTHESIZE(float, _animationDuration, AnimationDuration);
    CC_SYNTHESIZE(int, _repeatCount, RepeatCount);
};

#endif /* PulsingHaloLayer_hpp */
