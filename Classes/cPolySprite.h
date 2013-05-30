
#ifndef __cardmap__cPolySprite__
#define __cardmap__cPolySprite__

#include "cocos2d.h"

USING_NS_CC;

//����ξ���
class cPolySprite : public cocos2d::CCSprite
{
public:
    cPolySprite() : vertexs_(NULL), uvs_(NULL), indices_(NULL), verCnt_(0) {}
    virtual ~cPolySprite();
    
    static cPolySprite* create(const char *pFile,
                               const cocos2d::CCPoint *uvs,
                               int verCnt,
                               const int *indices);
    
    //���ظ���draw
    virtual void draw();
    //��ʼ��������Ϣ
    bool initWithUV(const cocos2d::CCPoint *uvs,
                    const int *indices,
                    int verCnt);
    
private:
    
    //�����е�
    cocos2d::CCPoint getCenter();
    void translate(const cocos2d::CCPoint&);
    void drawPoly();
    void releasePoly();
	bool retainPoly(int verCnt);
    
private:
    //����ζ���
    cocos2d::ccVertex2F *vertexs_;
    //������������
    cocos2d::ccVertex2F *uvs_;
    //����������
    unsigned short *indices_;
    //������ɫ
    unsigned char *colors_;
    //������Ŀ
    int verCnt_;
    
};

class MYCCSprite:public CCSprite{
public:
	static MYCCSprite* create(const char* pszFileName);
	virtual void draw();
};


////
// BYGraySprite.h
// Demo
//
// Created by Yanghui Liu on 12-11-2.
// Copyright (c) 2012�� BoyoJoy. All rights reserved.
//


class BYGraySprite : public CCSprite{


public:
 BYGraySprite();
 virtual ~BYGraySprite();
 static BYGraySprite* create(const char* pszFileName);
 bool initWithTexture(CCTexture2D* pTexture, const CCRect& tRect);
 virtual void draw();


};

#endif