//
//  MultiplePulsingHaloLayer.hpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/13/15.
//
//

#ifndef MultiplePulsingHaloLayer_hpp
#define MultiplePulsingHaloLayer_hpp

#include <stdio.h>

USING_NS_CC;

class MultiplePulsingHaloLayer: public Node {
    
public:
    
    static MultiplePulsingHaloLayer* create(const cocos2d::Color4B &color = Color4B(0, 122, 255, 255), float radius = 300,int repeatCount = 0);
    
    bool initWithColor(const cocos2d::Color4B &color, float radius = 300, int repeatCount = 0);
    void buildSubLayers();
    void start();
    
    CC_SYNTHESIZE(float, _radius, Radius);
    CC_SYNTHESIZE(Color4B, _haloLayerColor, HaloLayerColor);
    CC_SYNTHESIZE(float, _fromValueForRadius, FromValueForRadius);
    CC_SYNTHESIZE(float, _fromValueForAlpha, FromValueForAlpha);
    CC_SYNTHESIZE(float, _keyTimeForHalfOpacity, KeyTimeForHalfOpacity);
    CC_SYNTHESIZE(float, _animationDuration, AnimationDuration);
    CC_SYNTHESIZE(float, _startInterval, StartInterval);
    CC_SYNTHESIZE(float, _animationRepeatCount, AnimationRepeatCount);
    CC_SYNTHESIZE(int, _haloLayerNumber, HaloLayerNumber);
};

#endif /* MultiplePulsingHaloLayer_hpp */
