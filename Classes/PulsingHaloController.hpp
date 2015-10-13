//
//  PulsingHaloController.hpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/13/15.
//
//

#ifndef PulsingHaloController_hpp
#define PulsingHaloController_hpp

#include <stdio.h>
#include "extensions/cocos-ext.h"
#include "cocostudio/CocoStudio.h"

class PulsingHaloController : public cocostudio::ComController {
protected:
    PulsingHaloController();
    virtual ~PulsingHaloController();
    
public:
    virtual bool init() override;
    virtual void onAdd() override;
    virtual void onRemove() override;
    virtual void update(float delta) override;
    
    static PulsingHaloController* create();
    void reset();
    
public:
    void start();
    
    CC_SYNTHESIZE(float, _fromValueForRadius, FromValueForRadius);
    CC_SYNTHESIZE(float, _fromValueForAlpha, FromValueForAlpha);
    CC_SYNTHESIZE(float, _keyTimeForHalfOpacity, KeyTimeForHalfOpacity);
    CC_SYNTHESIZE(float, _animationDuration, AnimationDuration);
    CC_SYNTHESIZE(float, _delayInterval, DelayInterval);
    CC_SYNTHESIZE(int, _repeatCount, RepeatCount);
};

#endif /* PulsingHaloController_hpp */
