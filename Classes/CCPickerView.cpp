//
//  CCPickerView.cpp
//  pickerview
//
//  Created by 卓锐 on 16/2/20.
//
//

#include "CCPickerView.h"

CCPickerView::CCPickerView()
:m_pDataSource(NULL)
,m_pDelegate(NULL)
{
    CCLOG("CCPickerView::CCPickerView()");
}

CCPickerView::~CCPickerView()
{
    CCLOG("CCPickerView::~CCPickerView()");
}

CCPickerView *CCPickerView::createWithSize(CCSize size)
{
    CCPickerView* pRet = new CCPickerView();
    if (pRet && pRet->initWithSize(size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCPickerView::initWithSize(CCSize size)
{
    if (CCLayer::init())
    {
        this->setContentSize(size);
        m_bRepeatNodes = true;
        return true;
    }
    return false;
}

void CCPickerView::initialLoad()
{
    CCSize size = this->getContentSize();
//    float spacing = m_pDelegate->spaceBetweenComponents(this);
//    CCSprite* background;
//    background = CCSprite::create();
//    background->setColor(ccWHITE);
//    background->setTextureRect(CCRectMake(0, 0, size.width-spacing, size.height-spacing));
//    this->addChild(background);
    
    CCNode *overlayImage = m_pDelegate->overlayImage(this);
    if (overlayImage)
        this->addChild(overlayImage);
    this->reloadAllComponents();
}

void CCPickerView::setDelegate(CCPickerViewDelegate *newDelegate)
{
    m_pDelegate = newDelegate;
    
//    if (m_pDataSource != NULL) {
//        this->initialLoad();
//    }
}

void CCPickerView::setDataSource(CCPickerViewDataSource *newDataSource)
{
    m_pDataSource = newDataSource;
    
//    if (m_pDelegate != NULL) {
//        this->initialLoad();
//    }
}

void CCPickerView::autoRepeatNodes(bool repeat) {
    m_bRepeatNodes = repeat;
    this->reloadAllComponents();
}

void CCPickerView::spinComponent(int component, float speed, float rate, int repeat, int stopRow)
{
    if (m_bRepeatNodes) {
        CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
        scrollLayer->spin(speed, rate, repeat, stopRow);
    } else {
        CCLOG("You need to turn on autoRepeatNodes");
    }
}

void CCPickerView::spinComonentForever(int component, float speed, float rate)
{
    if (m_bRepeatNodes) {
        CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
        scrollLayer->spinForever(speed, rate);
    } else {
        CCLOG("You need to turn on autoRepeatNodes");
    }
}

int CCPickerView::numberOfRowsInComponent(int component) {
    CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
    
    return scrollLayer->getArrPages()->count();
}

void CCPickerView::reloadAllComponents()
{
    for (int c = 0; c < m_pDataSource->numberOfComponentsInPickerView(this); c++) {
        this->reloadComponent(c);
    }
}

void CCPickerView::reloadComponent(int component) {
    int numComponents = m_pDataSource->numberOfComponentsInPickerView(this);
    float componentsWidth = 0;
    int pickerStart = 0;
    float spacing = m_pDelegate->spaceBetweenComponents(this);
    CCSize componentSize = CCSizeMake(m_pDelegate->rowWidthForComponent(this, component), m_pDelegate->rowHeightForComponent(this, component));
    
    this->removeChildByTag(component, true);
    
    CCPickerScrollLayer *scrollLayer = CCPickerScrollLayer::create(componentSize);
    scrollLayer->setDelegate(this);
    
    for (int i = 0; i < numComponents; i++) {
        componentsWidth += m_pDelegate->rowWidthForComponent(this, component);
    }
    
    componentsWidth += (numComponents-1)*spacing;
    
    pickerStart = -componentsWidth/2;
    for (int c = 0; c < component; c++) {
        CCSize componentSize = CCSizeMake(m_pDelegate->rowWidthForComponent(this, c), m_pDelegate->rowHeightForComponent(this, c));
        pickerStart += componentSize.width + spacing;
    }
    
    
    scrollLayer->setPosition(ccp(pickerStart+componentSize.width/2, 0));
    
    CCArray *array = CCArray::createWithCapacity(10);
    int pageSize = m_pDataSource->numberOfRowsInComponent(this, component);
    
    // Duplicate the nodes if we have repeatNodes on.
    int numPages = m_bRepeatNodes?3:1;
    
    for (int r = 0; r < numPages; r++) {
        for (int p = 0; p < pageSize; p++) {
            array->addObject(m_pDelegate->nodeForRow(this, p, component));
        }
    }
    
    scrollLayer->setArrPages(array);
    scrollLayer->setPageSize(pageSize);
    scrollLayer->setCurrentPage(0, true);
    scrollLayer->setTouchSize(CCSizeMake(componentSize.width, this->getContentSize().height));
    scrollLayer->makePages();

    this->addChild(scrollLayer, 0, component);
}

void CCPickerView::reloadNodeInComponent(int component)
{
    int pageSize = m_pDataSource->numberOfRowsInComponent(this, component);
    int numPages = m_bRepeatNodes?3:1;
    CCArray *array = CCArray::createWithCapacity(10);
    for (int r = 0; r < numPages; r++) {
        for (int p = 0; p < pageSize; p++) {
            array->addObject(m_pDelegate->nodeForRow(this, p, component));
        }
    }
    CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
    scrollLayer->setArrPages(array);
    scrollLayer->reloadNode();
}

CCSize CCPickerView::rowSizeForComponent(int component)
{
    CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
    
    return scrollLayer->getContentSize();
}

int CCPickerView::selectedRowInComponent(int component)
{
    CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
    
    return scrollLayer->getCurrentPage();
}

void CCPickerView::selectRow(int row, int component, bool animated)
{
    CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
    scrollLayer->setCurrentPage(row, animated);
}

CCNode *CCPickerView::nodeForRow(int row, int component)
{
    CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(component);
    return scrollLayer->getChildByTag(row);
}

void CCPickerView::visit()
{
    if (!this->isVisible())
        return;
    
    glEnable(GL_SCISSOR_TEST);
    
    CCSize size =  this->getContentSize();
    CCPoint worldPoint = this->convertToWorldSpace(this->getPosition());
    CCRect rect = CCRectMake(worldPoint.x - this->getPositionX() - size.width/2, worldPoint.y - this->getPositionY() - size.height/2, size.width, size.height);
    float scaleX = CCEGLView::sharedOpenGLView()->getScaleX();
    float scaleY = CCEGLView::sharedOpenGLView()->getScaleY();
    
    CCRect   viewPortRect =  CCEGLView::sharedOpenGLView()->getViewPortRect();
    
    glScissor(rect.origin.x *scaleX  +  viewPortRect.origin.x  ,
              rect.origin.y * scaleY  +  viewPortRect.origin.y ,
              rect.size.width*scaleX,
              rect.size.height*scaleY);
    
    
    CCNode::visit();//显示父类的内容
    glDisable(GL_SCISSOR_TEST);}

void CCPickerView::onDoneSelecting(CCPickerScrollLayer *scrollLayer)
{
    m_pDelegate->didSelectRow(this, scrollLayer->getCurrentPage(), scrollLayer->getTag());
}

void CCPickerView::onDoneSpinning(CCPickerScrollLayer *scrollLayer) {
    m_pDelegate->onDoneSpinning(this, scrollLayer->getTag());
}

void CCPickerView::setScrollTouchEnable(bool isTouch)
{
    for (int c = 0; c < m_pDataSource->numberOfComponentsInPickerView(this); c++) {
        CCPickerScrollLayer *scrollLayer = (CCPickerScrollLayer *)this->getChildByTag(c);
        scrollLayer->setTouchEnabled(isTouch);
    }
}


void CCPickerView::registerScriptHandler(int nFunID,int nScriptEventType)
{
    this->unregisterScriptHandler(nScriptEventType);
    m_mapScriptHandler[nScriptEventType] = nFunID;
}

void CCPickerView::unregisterScriptHandler(int nScriptEventType)
{
    std::map<int,int>::iterator iter = m_mapScriptHandler.find(nScriptEventType);
    
    if (m_mapScriptHandler.end() != iter)
    {
        m_mapScriptHandler.erase(iter);
    }
}

int CCPickerView::getScriptHandler(int nScriptEventType)
{
    std::map<int,int>::iterator iter = m_mapScriptHandler.find(nScriptEventType);
    
    if (m_mapScriptHandler.end() != iter)
        return iter->second;
    
    return 0;
}

void CCPickerView::unregisterAllScriptHandler()
{
    unregisterScriptHandler(kNumberOfComponentsInPickerView);
    unregisterScriptHandler(kNumberOfRowsInComponent);
    unregisterScriptHandler(kPickerViewDidSelectRow);
    unregisterScriptHandler(kPickerViewNodeForRow);
    unregisterScriptHandler(kPickerViewOnDoneSpinning);
    unregisterScriptHandler(kPickerViewOverlayImage);
    unregisterScriptHandler(kPickerViewRowHeight);
    unregisterScriptHandler(kPickerViewRowWidth);
    unregisterScriptHandler(kPickerViewSpaceBetweenComponents);
}
