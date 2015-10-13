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

class PulsingHaloEffect: public Node, public BlendProtocol {
protected:
    int         _bufferCapacity;
    GLsizei     _bufferCount;
    V2F_C4B_T2F *_buffer;
    bool        _dirty;
    GLuint      _vao;
    GLuint      _vbo;
    
    BlendFunc   _blendFunc;
    CustomCommand _customCommand;
    
    float _radius;
    void prepairEffect();

    void onDraw(const Mat4& transform, uint32_t flags);
    virtual void updateColor() override;
    
public:
    PulsingHaloEffect();
    ~PulsingHaloEffect();
    
    static PulsingHaloEffect* create(const cocos2d::Color4B &color = Color4B(0, 122, 255, 255), float repeatCount = 0);
    
    bool initWithColor(const cocos2d::Color4B &color, float repeatCount);
    void start();
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    void drawDot(const Vec2 &pos, float radius, const Color4F &color);
    void ensureCapacity(int count);
    
    virtual void setBlendFunc(const BlendFunc &blendFunc) override;
    virtual const BlendFunc &getBlendFunc() const override;
    
    CC_SYNTHESIZE(Color4B, _haloColor, HaloColor);
    CC_SYNTHESIZE(float, _fromValueForRadius, FromValueForRadius);
    CC_SYNTHESIZE(float, _fromValueForAlpha, FromValueForAlpha);
    CC_SYNTHESIZE(float, _keyTimeForHalfOpacity, KeyTimeForHalfOpacity);
    CC_SYNTHESIZE(float, _animationDuration, AnimationDuration);
    CC_SYNTHESIZE(float, _pulseInterval, PulseInterval);
    CC_SYNTHESIZE(int, _repeatCount, RepeatCount);
    CC_SYNTHESIZE(bool, _useTimingFunction, UseTimingFunction);
};

#endif /* PulsingHaloLayer_hpp */
