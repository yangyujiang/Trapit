//
//  cPolySprite.cpp
//  cardmap
//
//  Created by sunny on 12-12-4.
//
//

#include "cPolySprite.h"
#include "cocos2d.h"

using namespace cocos2d;

cPolySprite* cPolySprite::create(const char *pFile,
                                 const cocos2d::CCPoint *uvs,
                                 int verCnt,
                                 const int *indices)
{
    cPolySprite *pobSprite = new cPolySprite();
    //创建精灵
    if (pobSprite &&
        pobSprite->initWithFile(pFile) &&
		pobSprite->retainPoly(verCnt) &&
        pobSprite->initWithUV(uvs, indices, verCnt)) {
        
        pobSprite->autorelease();
        return pobSprite;
    }
    CC_SAFE_DELETE(pobSprite);
    return false;
}

cPolySprite::~cPolySprite()
{
    releasePoly();
}
bool cPolySprite::retainPoly(int verCnt){
	if(!vertexs_) CC_SAFE_DELETE(vertexs_);
	if(!uvs_) CC_SAFE_DELETE(uvs_);
    if(!indices_) CC_SAFE_DELETE(indices_);
    if(!colors_) CC_SAFE_DELETE(colors_);
    //内存分配
    vertexs_ = new ccVertex2F[verCnt];
    uvs_     = new ccVertex2F[verCnt];
    indices_ = new unsigned short[(verCnt-2)*3];
    colors_  = new unsigned char[verCnt*4];
	
    //失败处理
    if(!vertexs_ || !uvs_ || !indices_ || !colors_) {
        releasePoly();
        return false;
    }
	return true;
}

//初始化顶点信息
bool cPolySprite::initWithUV(const cocos2d::CCPoint *uvs,
                             const int *indices,
                             int verCnt)
{
    //贴图大小
    CCSize rc = m_pobTexture->getContentSize();
    
    for(int i = 0; i < verCnt; ++i) {
        //根据纹理坐标以及纹理大小计算顶点坐标
        vertexs_[i].x = uvs[i].x*rc.width;
        //cocos2dx纹理坐标以左上角为原点
        vertexs_[i].y = (1.0-uvs[i].y)*rc.height;
        
        uvs_[i].x = uvs[i].x;
        uvs_[i].y = uvs[i].y;
    }
    
    for(int i = 0; i < (verCnt-2)*3; ++i)
        indices_[i] = indices[i];
    
    memset(colors_, 255, sizeof(unsigned char)*verCnt*4);
    
    verCnt_ = verCnt;
    
    translate(getCenter());

//	if(!uvs) CC_SAFE_DELETE(uvs);
//	if(!indices) CC_SAFE_DELETE(indices);
    
    return true;
}

//计算中点
CCPoint cPolySprite::getCenter()
{
    if(!vertexs_) return ccp(0,0);
    
    float minx = vertexs_[0].x,
          maxx = vertexs_[0].x,
          miny = vertexs_[0].y,
          maxy = vertexs_[0].y;
    
    //计算所有顶点坐标的中心点坐标
    for(int i = 0; i < verCnt_; ++i) {
        minx = minx>vertexs_[i].x?vertexs_[i].x:minx;
        maxx = maxx<vertexs_[i].x?vertexs_[i].x:maxx;
        
        miny = miny>vertexs_[i].y?vertexs_[i].y:miny;
        maxy = maxy<vertexs_[i].y?vertexs_[i].y:maxx;
    }
    
    return ccp((minx+maxx)*0.5, (miny+maxy)*0.5);
}

void cPolySprite::translate(const cocos2d::CCPoint& pos)
{
    //设置锚点
    CCSize rc = m_pobTexture->getContentSize();
    //setAnchorPoint(ccp(pos.x/rc.width, pos.y/rc.height));
	this->setAnchorPoint(ccp(0.5,0.5));
}

void cPolySprite::drawPoly()
{
    CC_NODE_DRAW_SETUP();
    
    ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );
    
    if (m_pobTexture != NULL) {
        ccGLBindTexture2D( m_pobTexture->getName() );
    }
    else {
        ccGLBindTexture2D(0);
    }
    
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex);
    
    //顶点，纹理，颜色
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertexs_);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uvs_);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors_);
    //根据索引draw三角形
    glDrawElements(GL_TRIANGLES, (verCnt_-2)*3, GL_UNSIGNED_SHORT, indices_);
    
    CC_INCREMENT_GL_DRAWS(1);
}

void cPolySprite::releasePoly()
{
    CC_SAFE_DELETE(vertexs_);
    CC_SAFE_DELETE(uvs_);
    CC_SAFE_DELETE(indices_);
    CC_SAFE_DELETE(colors_);
}

void cPolySprite::draw(void)
{
    drawPoly();
}



//
// BYGraySprite.cpp
// Demo
//
// Created by Yanghui Liu on 12-11-2.
// Copyright (c) 2012年 BoyoJoy. All rights reserved.
//


BYGraySprite::BYGraySprite(){


}


BYGraySprite::~BYGraySprite(){


}


BYGraySprite* BYGraySprite::create( const char* pszFileName ){
 BYGraySprite* graySprite = new BYGraySprite;
 if (graySprite && graySprite->initWithFile(pszFileName)){
 graySprite->autorelease();
 return graySprite;
 }else{
 CC_SAFE_RELEASE(graySprite);
 return NULL;
 }
}


bool BYGraySprite::initWithTexture(CCTexture2D* pTexture, const CCRect& tRect ){
 do{
 CC_BREAK_IF(!CCSprite::initWithTexture(pTexture, tRect));


 GLchar* pszFragSource =
 "#ifdef GL_ES \n \
 precision mediump float; \n \
 #endif \n \
 uniform sampler2D u_texture; \n \
 varying vec2 v_texCoord; \n \
 varying vec4 v_fragmentColor; \n \
 void main(void) \n \
 { \n \
 // Convert to greyscale using NTSC weightings \n \
 vec4 col = texture2D(u_texture, v_texCoord); \n \
float grey = dot(col.rgb, vec3(0.299, 0.587, 0.114)); \n \
gl_FragColor = vec4(grey, grey, grey, col.a); \n \
 }";


 CCGLProgram* pProgram = new CCGLProgram();
 pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, pszFragSource);
 this->setShaderProgram(pProgram);
 pProgram->release();
 CHECK_GL_ERROR_DEBUG();


 this->getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
 this->getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
 this->getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
 CHECK_GL_ERROR_DEBUG();


 this->getShaderProgram()->link();
 CHECK_GL_ERROR_DEBUG();


 this->getShaderProgram()->updateUniforms();
 CHECK_GL_ERROR_DEBUG();


 return true;
 } while (0);
 return false;
}


void BYGraySprite::draw(){
 ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
 ccGLBlendFunc( m_sBlendFunc.src, m_sBlendFunc.dst );


 this->getShaderProgram()->use();
 this->getShaderProgram()->setUniformForModelViewProjectionMatrix();


 ccGLBindTexture2D( this->getTexture()->getName() );


#define kQuadSize sizeof(m_sQuad.bl)
 long offset = (long)&m_sQuad;


 // vertex
 int diff = offsetof( ccV3F_C4B_T2F, vertices);
 glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));


 // texCoods
 diff = offsetof( ccV3F_C4B_T2F, texCoords);
 glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));


 // color
 diff = offsetof( ccV3F_C4B_T2F, colors);
 glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
 glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
 CC_INCREMENT_GL_DRAWS(1);
}