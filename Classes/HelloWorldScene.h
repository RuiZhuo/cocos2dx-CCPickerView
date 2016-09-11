#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "CCPickerView.h"

class HelloWorld : public cocos2d::CCLayer, CCPickerViewDelegate, CCPickerViewDataSource
{
public:
    char *test;
    CCPickerView *pickerView;
    CCLabelTTF *feedbackLabel0;
    CCLabelTTF *feedbackLabel1;
    CCLabelTTF *feedbackLabel2;
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
    
    // implement the "static node()" method manually
    static HelloWorld *create();
    
    void displayMainMenu();
    void spinPicker();
    void spinPickerSlow();
    void spinPickerEase();
    
    
    virtual int numberOfComponentsInPickerView(CCPickerView *pickerView);
    virtual int numberOfRowsInComponent(CCPickerView *pickerView, int component);
    virtual float rowHeightForComponent(CCPickerView * pickerView, int component);
    virtual float rowWidthForComponent(CCPickerView * pickerView, int component);
    virtual const char *titleForRow(CCPickerView * pickerView, int row, int component) { return "";};
    virtual CCNode *nodeForRow(CCPickerView * pickerView, int row, int component);
    virtual void didSelectRow(CCPickerView * pickerView, int row, int component);
    virtual float spaceBetweenComponents(CCPickerView *pickerView);
    virtual CCNode *overlayImage(CCPickerView *pickerView);
    virtual void onDoneSpinning(CCPickerView * pickerView, int component);
};

#endif // __HELLOWORLD_SCENE_H__
