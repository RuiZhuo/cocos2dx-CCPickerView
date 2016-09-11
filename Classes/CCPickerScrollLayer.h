//
//  CCPickerScrollLayer.h
//  pickerview
//
//  Created by 卓锐 on 16/2/20.
//
//

#ifndef CCPickerScrollLayer_h
#define CCPickerScrollLayer_h

#include <stdio.h>
#include "cocos2d.h"
USING_NS_CC;

#endif /* CCPickerScrollLayer_h */

class CCPickerScrollLayer;

class CCPickerScrollLayerDelegate
{
public:
    virtual ~CCPickerScrollLayerDelegate() {}
    virtual void onDoneSelecting(CCPickerScrollLayer *scrollLayer) = 0;
    virtual void onDoneSpinning(CCPickerScrollLayer *scrollLayer) = 0;
};

class CCPickerScrollLayer : public CCLayer{
public:
    CCPickerScrollLayer();
    virtual ~CCPickerScrollLayer();
    static CCPickerScrollLayer *create(CCSize size);
    bool initWithViewSize(CCSize size);
    virtual void registerWithTouchDispatcher();
    void makePages();
    void spin(float speed, float rate, int repeat, int stopPage);
    void spinForever(float speed, float rate);
    
    void setArrPages(CCArray *pPages);
    CCArray *getArrPages() { return m_pArrPages; }
    void setPageSize(int count) { m_nPageCount = count; }
    void setTouchSize(CCSize size) {m_touchSize = size; }
    void setDelegate(CCPickerScrollLayerDelegate *pDelegate){ m_pDelegate = pDelegate; };
    void setCurrentPage(int a, bool animate);
    int getCurrentPage();
    void reloadNode();
protected:
    int m_nPageCount;
    int m_nCurrentPage;
    CCArray* m_pArrPages;
    CCLayer* m_pLayWorld;
    CCSize m_touchSize;
    CCPoint m_touchStartedPoint;
    CCPoint m_touchStartedWorldPosition;
    bool m_bTouching;
    bool m_bMove;
    CCPickerScrollLayerDelegate *m_pDelegate;
    
    bool containsTouchLocation(CCTouch *pTouch);
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
    virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);
    void moveToPagePosition(bool animated);
    void onDoneSpinning();
};