//
//  CCPickerScrollLayer.cpp
//  pickerview
//
//  Created by 卓锐 on 16/2/20.
//
//

#include "CCPickerScrollLayer.h"

CCPickerScrollLayer::CCPickerScrollLayer()
:m_pLayWorld(NULL)
,m_pDelegate(NULL)
,m_pArrPages(NULL)
{
    CCLOG("CCPickerScrollLayer::CCPickerScrollLayer()");
}

CCPickerScrollLayer::~CCPickerScrollLayer()
{
    CCLOG("CCPickerScrollLayer::~CCPickerScrollLayer()");
    m_pArrPages->removeAllObjects();
    m_pArrPages->release();
}

CCPickerScrollLayer* CCPickerScrollLayer::create(CCSize size)
{
    CCPickerScrollLayer* pRet = new CCPickerScrollLayer();
    if (pRet && pRet->initWithViewSize(size))
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

bool CCPickerScrollLayer::initWithViewSize(CCSize size)
{
    if (CCLayer::init())
    {
        this->setTouchEnabled(true);
        m_bTouching = false;
        this->setContentSize(size);
        
        return true;
    }
    return false;
}

void CCPickerScrollLayer::registerWithTouchDispatcher()
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, CCLayer::getTouchPriority(), false);
}

void CCPickerScrollLayer::setArrPages(CCArray *pPages)
{
    if (m_pArrPages)
    {
        m_pArrPages->removeAllObjects();
        m_pArrPages->release();
    }
    m_pArrPages = pPages;
    m_pArrPages->retain();
}

void CCPickerScrollLayer::makePages()
{
    CCSize s = this->getContentSize();
    //	CCLOG(@"[%f][%f]",s.width,s.height);
    //	CCLOG(@"anchorpoint[%f][%f]",self.anchorPoint.x,self.anchorPoint.y);
    m_pLayWorld = CCLayer::create();
    m_pLayWorld->setContentSize(CCSizeMake(s.width, s.height));
    for (int i=0; i < m_pArrPages->count(); i++) {
        CCNode* n = (CCNode *)m_pArrPages->objectAtIndex(i);
        n->setPosition(ccp(s.width / 2, s.height / 2 + i * s.height));
        m_pLayWorld->addChild(n, 0, i);
    }
    //world.anchorPoint = ccp(0,0);
    m_pLayWorld->setPosition(ccp(-s.width/2, -s.height/2 -s.height * m_nCurrentPage));
    this->addChild(m_pLayWorld);
}

bool CCPickerScrollLayer::containsTouchLocation(CCTouch *pTouch)
{
    CCSize contentSize = this->getContentSize();
    CCPoint p = this->convertTouchToNodeSpaceAR(pTouch);
    CCRect r = CCRectMake(-m_touchSize.width, -contentSize.height/2 - m_touchSize.height/2, m_touchSize.width, m_touchSize.height);
    return r.containsPoint(p);
}

bool CCPickerScrollLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
    if (m_bTouching) return false;
    if ( ! this->containsTouchLocation(touch) ) return false;
    m_bTouching = true;
    m_touchStartedPoint = this->convertTouchToNodeSpaceAR(touch);
    m_touchStartedWorldPosition = m_pLayWorld->getPosition();
    CCLOG("touch handle");
    return true;
}

void CCPickerScrollLayer::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    m_bMove = true;
    CCPoint n = this->convertTouchToNodeSpaceAR(touch);
    m_pLayWorld->setPosition(ccp(m_touchStartedWorldPosition.x, m_touchStartedWorldPosition.y + n.y - m_touchStartedPoint.y));
    CCLOG("moved %f %f", m_pLayWorld->getPositionX(), m_pLayWorld->getPositionY());
}

void CCPickerScrollLayer::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCSize contentSize = this->getContentSize();
    CCPoint n = this->convertTouchToNodeSpaceAR(touch);
    int pagesToMove = round((n.y - m_touchStartedPoint.y) / contentSize.height);
    
    if (!m_bMove) {
        if (n.y < -contentSize.height) {
            pagesToMove = 1;
        }
        if (n.y > 0) {
            pagesToMove = -1;
        }
    }
    m_bMove = false;
    m_nCurrentPage = this->getCurrentPage() - pagesToMove;
//    m_nCurrentPage = this->getCurrentPage();
    this->setCurrentPage(m_nCurrentPage, true);
    m_bTouching = false;
    m_pDelegate->onDoneSelecting(this);
    
    CCLOG("ended %f %f", m_pLayWorld->getPositionX(), m_pLayWorld->getPositionY());
}

void CCPickerScrollLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event)
{
    m_bTouching = false;
    this->moveToPagePosition(true);
}

int CCPickerScrollLayer::getCurrentPage()
{
    // Always return a value in the first page.
    int retVal = m_nCurrentPage % m_nPageCount;
    CCLOG("currentPage %d, retVal %d", m_nCurrentPage, retVal);
    return retVal;
}

void CCPickerScrollLayer::setCurrentPage(int a, bool animated) {
    
    // If arrayPages is bigger than pageSize+1 then we must have duplicate records to enable a continuous display, so change the value to be on the second set of values.
    if (m_nPageCount != m_pArrPages->count()) {
        a += m_nPageCount;
    } else {
        // We do not have a continue display so make sure that we are within the range.
        if (a < 0) {
            a = 0;
        }
        
        if (a > m_nPageCount-1) {
            a = m_nPageCount-1;
        }
    }
    
    m_nCurrentPage = a;
    if (m_pLayWorld) {
        this->moveToPagePosition(animated);
    }
}

void CCPickerScrollLayer::moveToPagePosition(bool animated) {
    CCPoint positionNow = m_pLayWorld->getPosition();
    CCSize s = this->getContentSize();
    float diffY = fabs( (positionNow.y) - (-s.height /2 -s.height * m_nCurrentPage) );
    CCLOG("positionNow %f, diff[%f]",positionNow.y, diffY);
    
    CCFiniteTimeAction *moveAction = NULL;
    CCFiniteTimeAction *instantAction = NULL;

    // If arrayPages is bigger than pageSize+1 then we must have duplicate records to enable a continuous display, so change the value to be on the second set of values.
    if (m_nPageCount != m_pArrPages->count()) {
        // Make sure we have not moved too far for the continuous display.  If so we want to move back to the second set of values, but without the user seeing the change.
        if (m_nCurrentPage >= m_nPageCount*2) {
            m_nCurrentPage -= m_nPageCount;
            instantAction = CCMoveTo::create(0, ccp(-s.width /2, -s.height/2 - s.height * m_nCurrentPage));
        }
        
        if (m_nCurrentPage < m_nPageCount) {
            m_nCurrentPage += m_nPageCount;
            instantAction = CCMoveTo::create(0, ccp(-s.width /2, -s.height/2 - s.height * m_nCurrentPage));
        }
    }
    
    if (animated)
    {
        if (diffY > 0) {
            moveAction = CCMoveTo::create((0.2 * diffY / s.height), ccp(-s.width /2, -s.height/2 - s.height * m_nCurrentPage));
        }
    }
    else
    {
        moveAction = CCMoveTo::create(0, ccp(-s.width /2, -s.height/2 - s.height * m_nCurrentPage));
    }
    CCSequence *sequence = CCSequence::create(moveAction, instantAction, NULL);
    if (sequence) {
        m_pLayWorld->stopAllActions();
        m_pLayWorld->runAction(sequence);
    }
}

void CCPickerScrollLayer::spin(float speed, float rate, int repeat, int stopPage)
{
    CCPoint positionNow = this->getPosition();
    CCSize s = this->getContentSize();
    
    speed = 1.0/speed;
    
    // Move to end of the second set of pages for the first time.
    float diffY = fabs( (positionNow.y) - (-s.height /2 -s.height * m_nPageCount * 2.0));
    CCFiniteTimeAction *firstMoveToPageEndAction = CCMoveTo::create((speed * diffY / s.height), ccp(-s.width /2, -s.height/2 - s.height * m_nPageCount * 2.0));
    
    // Move to the begining of the second set of pages, the user will not see this action.
    CCFiniteTimeAction *moveToPageStartAction = CCMoveTo::create(0, ccp(-s.width /2, -s.height/2 - s.height * m_nPageCount));
    
    // Move from the begining of the second set of pages to the end of the second set of pages.
    diffY = fabs( (-s.height /2 -s.height * m_nPageCount) - (-s.height /2 -s.height * m_nPageCount * 2.0));
    CCFiniteTimeAction *moveToPageEndAction = CCMoveTo::create((speed * diffY / s.height), ccp(-s.width /2, -s.height/2 - s.height * m_nPageCount * 2.0));
    
    // Move from the begining of the second set of pages to the final page, in the second set of pages.
    diffY = fabs( (-s.height /2 -s.height * m_nPageCount) - (-s.height /2 -s.height * (stopPage+m_nPageCount)));
    CCFiniteTimeAction *moveToFinalPageAction = CCMoveTo::create((speed * diffY / s.height), ccp(-s.width /2, -s.height/2 - s.height * (stopPage+m_nPageCount)));
    
    CCRepeat *sRepeat = CCRepeat::create(CCSequence::createWithTwoActions(moveToPageStartAction, moveToPageEndAction), repeat);
    
    CCCallFunc *callBack = CCCallFunc::create(this, callfunc_selector(CCPickerScrollLayer::onDoneSpinning));
    
    CCSequence *pSequence = CCSequence::create(firstMoveToPageEndAction, sRepeat, moveToPageStartAction,
                                               moveToFinalPageAction, callBack, NULL);
    
    m_pLayWorld->stopAllActions();
    m_pLayWorld->runAction(CCEaseInOut::create(pSequence, rate));
    
    m_nCurrentPage = stopPage+m_nPageCount;
    
}

void CCPickerScrollLayer::spinForever(float speed, float rate)
{
    CCPoint positionNow = this->getPosition();
    CCSize s = this->getContentSize();
    
    speed = 1.0/speed;
    
    // Move to end of the second set of pages for the first time.
    float diffY = fabs( (positionNow.y) - (-s.height /2 -s.height * m_nPageCount * 2.0));
//    CCFiniteTimeAction *firstMoveToPageEndAction = CCMoveTo::create((speed * diffY / s.height), ccp(-s.width /2, -s.height/2 - s.height * m_nPageCount * 2.0));
    
    // Move to the begining of the second set of pages, the user will not see this action.
    CCFiniteTimeAction *moveToPageStartAction = CCMoveTo::create(0, ccp(-s.width /2, -s.height/2 - s.height * m_nPageCount));
    
    // Move from the begining of the second set of pages to the end of the second set of pages.
    diffY = fabs( (-s.height /2 -s.height * m_nPageCount) - (-s.height /2 -s.height * m_nPageCount * 2.0));
    CCFiniteTimeAction *moveToPageEndAction = CCMoveTo::create((speed * diffY / s.height), ccp(-s.width /2, -s.height/2 - s.height * m_nPageCount * 2.0));
    CCRepeatForever *forever = CCRepeatForever::create(CCSequence::createWithTwoActions(moveToPageStartAction, moveToPageEndAction));
    //    CCSequence *pSequence = CCSequence::create(forever,  NULL);
    
    m_pLayWorld->stopAllActions();
    m_pLayWorld->runAction(forever);
    
}

void CCPickerScrollLayer::onDoneSpinning()
{
    m_pDelegate->onDoneSpinning(this);
}

void CCPickerScrollLayer::reloadNode()
{
    CCSize s = this->getContentSize();
    m_pLayWorld->removeAllChildrenWithCleanup(true);
    for (int i=0; i < m_pArrPages->count(); i++) {
        CCNode* n = (CCNode *)m_pArrPages->objectAtIndex(i);
        n->setPosition(ccp(s.width / 2, s.height / 2 + i * s.height));
        m_pLayWorld->addChild(n, 0, i);
    }
}
