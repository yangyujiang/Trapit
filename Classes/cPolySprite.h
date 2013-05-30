
#ifndef __cardmap__cPolySprite__
#define __cardmap__cPolySprite__

#include "cocos2d.h"

USING_NS_CC;

//多边形精灵
class cPolySprite : public cocos2d::CCSprite
{
public:
    cPolySprite() : vertexs_(NULL), uvs_(NULL), indices_(NULL), verCnt_(0) {}
    virtual ~cPolySprite();
    
    static cPolySprite* create(const char *pFile,
                               const cocos2d::CCPoint *uvs,
                               int verCnt,
                               const int *indices);
    
    //重载父类draw
    virtual void draw();
    //初始化顶点信息
    bool initWithUV(const cocos2d::CCPoint *uvs,
                    const int *indices,
                    int verCnt);
    
private:
    
    //计算中点
    cocos2d::CCPoint getCenter();
    void translate(const cocos2d::CCPoint&);
    void drawPoly();
    void releasePoly();
	bool retainPoly(int verCnt);
    
private:
    //多边形顶点
    cocos2d::ccVertex2F *vertexs_;
    //定点纹理坐标
    cocos2d::ccVertex2F *uvs_;
    //三角形索引
    unsigned short *indices_;
    //顶点颜色
    unsigned char *colors_;
    //顶点数目
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
// Copyright (c) 2012年 BoyoJoy. All rights reserved.
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