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
USING_NS_CC;

class PulsingHaloEffect: public LayerColor {
protected:
    float _radius;
    void prepairEffect();
    
public:
    PulsingHaloEffect();
    ~PulsingHaloEffect();
    
    static PulsingHaloEffect* create(const cocos2d::Color4B &color = Color4B(0, 122, 255, 255), float repeatCount = 0);
    
    bool initWithColor(const cocos2d::Color4B &color, float repeatCount);
    void start();
    void end(const Point point, const std::function<void(Node*)>& endFunc);
    
    CC_SYNTHESIZE(float, _fromValueForRadius, FromValueForRadius);
    CC_SYNTHESIZE(float, _fromValueForAlpha, FromValueForAlpha);
    CC_SYNTHESIZE(float, _keyTimeForHalfOpacity, KeyTimeForHalfOpacity);
    CC_SYNTHESIZE(float, _animationDuration, AnimationDuration);
    CC_SYNTHESIZE(float, _pulseInterval, PulseInterval);
    CC_SYNTHESIZE(int, _repeatCount, RepeatCount);
    CC_SYNTHESIZE(bool, _useTimingFunction, UseTimingFunction);
};

#endif /* PulsingHaloLayer_hpp */
