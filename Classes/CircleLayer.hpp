//
//  CircleLayer.hpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/13/15.
//
//

#ifndef CircleLayer_hpp
#define CircleLayer_hpp

#include <stdio.h>
USING_NS_CC;

class CircleLayer: public Node, public BlendProtocol {
protected:
    GLsizei     _bufferCount; // default by 6
    V2F_C4B_T2F *_buffer;
    bool        _dirty;
    GLuint      _vao;
    GLuint      _vbo;
    
    BlendFunc   _blendFunc;
    CustomCommand _customCommand;
    
    float _radius;
    void onDraw(const Mat4& transform, uint32_t flags);
    virtual void updateColor() override;
    
public:
    CircleLayer();
    ~CircleLayer();
    
    static CircleLayer* create(const cocos2d::Color4B &color = Color4B::BLUE, float radius = 10);
    
    bool initWithColor(const cocos2d::Color4B &color, float radius);
    
    virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;
    
    virtual void setBlendFunc(const BlendFunc &blendFunc) override;
    virtual const BlendFunc &getBlendFunc() const override;
};

#endif /* CircleLayer_hpp */
