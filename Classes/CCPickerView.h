//
//  CCPickerView.h
//  pickerview
//
//  Created by 卓锐 on 16/2/20.
//
//

#ifndef CCPickerView_h
#define CCPickerView_h

#include <stdio.h>
#include "cocos2d.h"
#include "CCPickerScrollLayer.h"
USING_NS_CC;

class CCPickerView;

class CCPickerViewDataSource {
public:
    virtual ~CCPickerViewDataSource() {}
    virtual int numberOfComponentsInPickerView(CCPickerView *pickerView) = 0;
    virtual int numberOfRowsInComponent(CCPickerView *pickerView, int component) = 0;
};

class CCPickerViewDelegate {
public:
    virtual ~CCPickerViewDelegate() {}
    virtual float rowHeightForComponent(CCPickerView * pickerView, int component) = 0;
    virtual float rowWidthForComponent(CCPickerView * pickerView, int component) = 0;
    virtual const char *titleForRow(CCPickerView * pickerView, int row, int component) { return "";};
    virtual CCNode *nodeForRow(CCPickerView * pickerView, int row, int component) = 0;
    virtual void didSelectRow(CCPickerView * pickerView, int row, int component) = 0;
    virtual float spaceBetweenComponents(CCPickerView *pickerView) = 0;
    virtual CCNode *overlayImage(CCPickerView *pickerView) = 0;
    virtual void onDoneSpinning(CCPickerView * pickerView, int component) = 0;
};

class CCPickerView : public CCLayer,  CCPickerScrollLayerDelegate{
public:
    CCPickerView();
    virtual ~CCPickerView();
    static CCPickerView *createWithSize(CCSize size);
    bool initWithSize(CCSize size);
    
    void setDataSource(CCPickerViewDataSource* source);
    CCPickerViewDataSource *getDataSource() {return m_pDataSource; };
    void setDelegate(CCPickerViewDelegate* pDelegate);
    CCPickerViewDelegate *getDelegate() {return m_pDelegate; }
    
    void setScrollTouchEnable(bool isTouch);
    void initialLoad();
    int numberOfRowsInComponent(int component);
    void reloadAllComponents();
    void reloadComponent(int component);
    CCSize rowSizeForComponent(int component);
    int selectedRowInComponent(int component);
    void selectRow(int row, int component, bool animated);
    CCNode *nodeForRow(int row, int component);
    void autoRepeatNodes(bool repeat);
    void spinComponent(int component, float speed, float rate, int repeat, int stopRow);
    void spinComonentForever(int component, float speed, float rate);
    void reloadNodeInComponent(int component);
    
    virtual void onDoneSelecting(CCPickerScrollLayer *scrollLayer);
    virtual void onDoneSpinning(CCPickerScrollLayer *scrollLayer);
    
public:
    enum PickerViewScriptEventType
    {
        kNumberOfComponentsInPickerView = 0,
        kNumberOfRowsInComponent,
        kPickerViewRowHeight,
        kPickerViewRowWidth,
        kPickerViewNodeForRow,
        kPickerViewDidSelectRow,
        kPickerViewSpaceBetweenComponents,
        kPickerViewOverlayImage,
        kPickerViewOnDoneSpinning,
    };
    void registerScriptHandler(int nFunID,int nScriptEventType);
    void unregisterScriptHandler(int nScriptEventType);
    int  getScriptHandler(int nScriptEventType);
    void unregisterAllScriptHandler();

protected:
    CCPickerViewDataSource *m_pDataSource;
    CCPickerViewDelegate *m_pDelegate;
    bool m_bRepeatNodes;
    int m_nNumberOfComponents;
    
    virtual void visit();
private:
    std::map<int,int> m_mapScriptHandler;
};

#endif /* CCPickerView_h */
