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

PulsingHaloEffect::PulsingHaloEffect():
_bufferCount(0),
_bufferCapacity(0),
_buffer(nullptr){
    
}

PulsingHaloEffect::~PulsingHaloEffect() {
    
}

bool PulsingHaloEffect::initWithColor(const cocos2d::Color4B &color, float repeatCount) {
    if (Node::init()) {
        _radius = 600;
        _fromValueForRadius = 0.0;
        _fromValueForAlpha = 0.45;
        _keyTimeForHalfOpacity = 0.2;
        _animationDuration = 3;
        _pulseInterval = 0;
        _repeatCount = repeatCount;
        _useTimingFunction = true;
        _haloColor = color;
        _displayedColor.r = _realColor.r = color.r;
        _displayedColor.g = _realColor.g = color.g;
        _displayedColor.b = _realColor.b = color.b;
        _displayedOpacity = _realOpacity = color.a;
        
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));
        
        ensureCapacity(512);
        
        if (Configuration::getInstance()->supportsShareableVAO())
        {
            glGenVertexArrays(1, &_vao);
            GL::bindVAO(_vao);
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
            // vertex
            glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_POSITION);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
            // color
            glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_COLOR);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
            // texcood
            glEnableVertexAttribArray(GLProgram::VERTEX_ATTRIB_TEX_COORD);
            glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
            
            GL::bindVAO(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
        }
        else
        {
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCapacity, _buffer, GL_STREAM_DRAW);
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
        
        CHECK_GL_ERROR_DEBUG();
        
        _dirty = true;
        
#if CC_ENABLE_CACHE_TEXTURE_DATA
        // Need to listen the event only when not use batchnode, because it will use VBO
        auto listener = EventListenerCustom::create(EVENT_RENDERER_RECREATED, [this](EventCustom* event){
            /** listen the event that renderer was recreated on Android/WP8 */
            this->init();
        });
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
#endif
        
        prepairEffect();
        
        return true;
    }
    
    return false;
}

/// blendFunc getter
const BlendFunc& PulsingHaloEffect::getBlendFunc() const
{
    return _blendFunc;
}
/// blendFunc setter
void PulsingHaloEffect::setBlendFunc(const BlendFunc &var)
{
    _blendFunc = var;
}

void PulsingHaloEffect::prepairEffect() {
    drawDot(Vec2(0, 0), _radius, Color4F(_haloColor));
    drawDot(Vec2(_radius, _radius), _radius, Color4F(_haloColor));
    
    this->setScale(_fromValueForRadius);
    this->setOpacity(_fromValueForAlpha * 255);
    
    updateColor();
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

void PulsingHaloEffect::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    if (_bufferCount) {
        _customCommand.init(_globalZOrder, transform, flags);
        _customCommand.func = CC_CALLBACK_0(PulsingHaloEffect::onDraw, this, transform, flags);
        renderer->addCommand(&_customCommand);
    }
}

void PulsingHaloEffect::onDraw(const Mat4& transform, uint32_t flags)
{
    auto glProgram = getGLProgram();
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCapacity, _buffer, GL_STREAM_DRAW);
        
        _dirty = false;
    }
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(_vao);
    }
    else
    {
        GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
        
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        // vertex
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, vertices));
        // color
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, colors));
        // texcood
        glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(V2F_C4B_T2F), (GLvoid *)offsetof(V2F_C4B_T2F, texCoords));
    }
    
    glDrawArrays(GL_TRIANGLES, 0, _bufferCount);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    if (Configuration::getInstance()->supportsShareableVAO())
    {
        GL::bindVAO(0);
    }
    
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1, _bufferCount);
    CHECK_GL_ERROR_DEBUG();
}

void PulsingHaloEffect::updateColor() {
    log("%d", _displayedOpacity);
    
    drawDot(Vec2::ZERO, _radius, Color4F(_displayedColor.r / 255.0f, _displayedColor.g / 255.0f, _displayedColor.b / 255.0f, _displayedOpacity / 255.0f));
}

void PulsingHaloEffect::drawDot(const Vec2 &pos, float radius, const Color4F &color)
{
    unsigned int vertex_count = 2*3;
    ensureCapacity(vertex_count);
    
    V2F_C4B_T2F a = {Vec2(pos.x - radius, pos.y - radius), Color4B(color), Tex2F(-1.0, -1.0) };
    V2F_C4B_T2F b = {Vec2(pos.x - radius, pos.y + radius), Color4B(color), Tex2F(-1.0,  1.0) };
    V2F_C4B_T2F c = {Vec2(pos.x + radius, pos.y + radius), Color4B(color), Tex2F( 1.0,  1.0) };
    V2F_C4B_T2F d = {Vec2(pos.x + radius, pos.y - radius), Color4B(color), Tex2F( 1.0, -1.0) };
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount - 6);
    V2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    V2F_C4B_T2F_Triangle triangle1 = {a, c, d};
    triangles[0] = triangle0;
    triangles[1] = triangle1;
    
    _bufferCount = vertex_count;
    
    _dirty = true;
}

void PulsingHaloEffect::ensureCapacity(int count)
{
    CCASSERT(count>=0, "capacity must be >= 0");
    
    if(_bufferCount + count > _bufferCapacity)
    {
        _bufferCapacity = MAX(_bufferCapacity, count);
        _buffer = (V2F_C4B_T2F*)realloc(_buffer, _bufferCapacity*sizeof(V2F_C4B_T2F));
    }
}