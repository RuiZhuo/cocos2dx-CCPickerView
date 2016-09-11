# 版本要求
cocos2dx-2.2.5,理论上2.x都可以。
# 使用方法
##1、CCPickerView的创建
```
#include "CCPickerView.h"
.
.
.

pickerView = CCPickerView::createWithSize(CCSizeMake(200, 100));
pickerView->setPosition(visibleSize.width*0.25 ,visibleSize.height/2);
pickerView->setDataSource(this);
pickerView->setDelegate(this);
pickerView->initialLoad();
```

##2、注册委托协议
```
//设置滚动列数
virtual int numberOfComponentsInPickerView(CCPickerView *pickerView);

//设置每列的行数
virtual int numberOfRowsInComponent(CCPickerView *pickerView, int component);

//设置列的高度
virtual float rowHeightForComponent(CCPickerView * pickerView, int component);

//设置列的宽度
virtual float rowWidthForComponent(CCPickerView * pickerView, int component);

//设置每个格子的label
virtual const char *titleForRow(CCPickerView * pickerView, int row, int component) { return "";};

//设置每个格子要添加的node
virtual CCNode *nodeForRow(CCPickerView * pickerView, int row, int component);

//选择时候的回调
virtual void didSelectRow(CCPickerView * pickerView, int row, int component);

//设置每列的间隔
virtual float spaceBetweenComponents(CCPickerView *pickerView);

//设置控件底图
virtual CCNode *overlayImage(CCPickerView *pickerView);

//设置滚动动画停止时的回调
virtual void onDoneSpinning(CCPickerView * pickerView, int component);
```

##3、播放动画
###自动暂停动画：
```
pickerView->spinComponent(int component, float speed, float rate, int repeat, int stopRow);
```

###手动暂停动画：
```
//播放动画
void spinComonentForever(int component, float speed, float rate);

//停止动画 animated填false
void CCPickerView::selectRow(int row, int component, bool animated)

```

