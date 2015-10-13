//
//  CircleLayer.cpp
//  XDPulsingHalo
//
//  Created by VuTT5 on 10/13/15.
//
//

#include "CircleLayer.hpp"

CircleLayer::CircleLayer():
_bufferCount(2 * 3),
_buffer(nullptr){
    
}

CircleLayer::~CircleLayer() {
    
}

CircleLayer* CircleLayer::create(const cocos2d::Color4B &color, float radius) {
    CircleLayer *pRet = new(std::nothrow) CircleLayer();
    
    if (pRet && pRet->initWithColor(color, radius))
    {
        pRet->autorelease();
        return pRet;
    } else {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool CircleLayer::initWithColor(const cocos2d::Color4B &color, float radius) {
    if (Node::init()) {
        _radius = radius;
        _displayedColor.r = _realColor.r = color.r;
        _displayedColor.g = _realColor.g = color.g;
        _displayedColor.b = _realColor.b = color.b;
        _displayedOpacity = _realOpacity = color.a;
        
        _blendFunc = BlendFunc::ALPHA_PREMULTIPLIED;
        
        setGLProgramState(GLProgramState::getOrCreateWithGLProgramName(GLProgram::SHADER_NAME_POSITION_LENGTH_TEXTURE_COLOR));
        
        _buffer = (V2F_C4B_T2F*)realloc(_buffer, _bufferCount*sizeof(V2F_C4B_T2F));
        
        if (Configuration::getInstance()->supportsShareableVAO())
        {
            glGenVertexArrays(1, &_vao);
            GL::bindVAO(_vao);
            glGenBuffers(1, &_vbo);
            glBindBuffer(GL_ARRAY_BUFFER, _vbo);
            glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCount, _buffer, GL_STREAM_DRAW);
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
            glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)* _bufferCount, _buffer, GL_STREAM_DRAW);
            
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
        
        updateColor();
        
        return true;
    }
    
    return false;
}

void CircleLayer::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) {
    _customCommand.init(_globalZOrder, transform, flags);
    _customCommand.func = CC_CALLBACK_0(CircleLayer::onDraw, this, transform, flags);
    renderer->addCommand(&_customCommand);
}

void CircleLayer::onDraw(const cocos2d::Mat4 &transform, uint32_t flags) {
    auto glProgram = getGLProgram();
    glProgram->use();
    glProgram->setUniformsForBuiltins(transform);
    
    GL::blendFunc(_blendFunc.src, _blendFunc.dst);
    
    if (_dirty)
    {
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(V2F_C4B_T2F)*_bufferCount, _buffer, GL_STREAM_DRAW);
        
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

void CircleLayer::updateColor() {
    Color4F color = Color4F(_displayedColor.r / 255.0f, _displayedColor.g / 255.0f, _displayedColor.b / 255.0f, _displayedOpacity / 255.0f);
    
    unsigned int vertex_count = 2 * 3;
    
    V2F_C4B_T2F a = {Vec2(- _radius, - _radius), Color4B(color), Tex2F(-1.0, -1.0) };
    V2F_C4B_T2F b = {Vec2(- _radius, _radius), Color4B(color), Tex2F(-1.0,  1.0) };
    V2F_C4B_T2F c = {Vec2(_radius, _radius), Color4B(color), Tex2F( 1.0,  1.0) };
    V2F_C4B_T2F d = {Vec2(_radius, - _radius), Color4B(color), Tex2F( 1.0, -1.0) };
    
    V2F_C4B_T2F_Triangle *triangles = (V2F_C4B_T2F_Triangle *)(_buffer + _bufferCount - 6);
    V2F_C4B_T2F_Triangle triangle0 = {a, b, c};
    V2F_C4B_T2F_Triangle triangle1 = {a, c, d};
    triangles[0] = triangle0;
    triangles[1] = triangle1;
    
    _bufferCount = vertex_count;
    
    _dirty = true;
}

void CircleLayer::setBlendFunc(const BlendFunc &blendFunc) {
    _blendFunc = blendFunc;
}

const BlendFunc& CircleLayer::getBlendFunc() const {
    return _blendFunc;
}