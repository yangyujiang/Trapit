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
    //��������
    if (pobSprite &&
        pobSprite->initWithFile(pFile) &&
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

//��ʼ��������Ϣ
bool cPolySprite::initWithUV(const cocos2d::CCPoint *uvs,
                             const int *indices,
                             int verCnt)
{
    //�ڴ����
    vertexs_ = new ccVertex2F[verCnt];
    uvs_     = new ccVertex2F[verCnt];
    indices_ = new unsigned short[(verCnt-2)*3];
    colors_  = new unsigned char[verCnt*4];
    
    //ʧ�ܴ���
    if(!vertexs_ || !uvs_ || !indices_ || !colors_) {
        releasePoly();
        return false;
    }
    
    //��ͼ��С
    CCSize rc = m_pobTexture->getContentSize();
    
    for(int i = 0; i < verCnt; ++i) {
        //�������������Լ������С���㶥������
        vertexs_[i].x = uvs[i].x*rc.width;
        //cocos2dx�������������Ͻ�Ϊԭ��
        vertexs_[i].y = (1.0-uvs[i].y)*rc.height;
        
        uvs_[i].x = uvs[i].x;
        uvs_[i].y = uvs[i].y;
    }
    
    for(int i = 0; i < (verCnt-2)*3; ++i)
        indices_[i] = indices[i];
    
    memset(colors_, 255, sizeof(unsigned char)*verCnt*4);
    
    verCnt_ = verCnt;
    
    translate(getCenter());
    
    return true;
}

//�����е�
CCPoint cPolySprite::getCenter()
{
    if(!vertexs_) return ccp(0,0);
    
    float minx = vertexs_[0].x,
          maxx = vertexs_[0].x,
          miny = vertexs_[0].y,
          maxy = vertexs_[0].y;
    
    //�������ж�����������ĵ�����
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
    //����ê��
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
    
    //���㣬������ɫ
    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertexs_);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0, uvs_);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, colors_);
    //��������draw������
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