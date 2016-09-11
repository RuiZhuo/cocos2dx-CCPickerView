#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

HelloWorld *HelloWorld::create()
{
    HelloWorld* pRet = new HelloWorld();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(pRet);
    }
    return pRet;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    test = NULL;
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

    pickerView = CCPickerView::createWithSize(CCSizeMake(200, 100));
    pickerView->setPosition(visibleSize.width*0.25 ,visibleSize.height/2);
    pickerView->setDataSource(this);
    pickerView->setDelegate(this);
    pickerView->initialLoad();
    this->addChild(pickerView);
    
    feedbackLabel0 = CCLabelTTF::create("Component 0 Stopped", "Helvetica", 32);
    feedbackLabel1 = CCLabelTTF::create("Component 1 Stopped", "Helvetica", 32);
    feedbackLabel2 = CCLabelTTF::create("Component 2 Stopped", "Helvetica", 32);
    feedbackLabel0->setColor(ccc3(255, 255, 255));
    feedbackLabel1->setColor(ccc3(255, 255, 255));
    feedbackLabel2->setColor(ccc3(255, 255, 255));
    
    feedbackLabel0->setPosition(ccp( visibleSize.width *0.8 , visibleSize.height*0.90 ));
    feedbackLabel1->setPosition(ccp( visibleSize.width *0.8 , visibleSize.height*0.85 ));
    feedbackLabel2->setPosition(ccp( visibleSize.width *0.8 , visibleSize.height*0.80 ));
    
    this->addChild(feedbackLabel0);
    this->addChild(feedbackLabel1);
    this->addChild(feedbackLabel2);
    
    this->displayMainMenu();
    
    return true;
}

void HelloWorld::displayMainMenu()
{
    CCSize screenSize = CCDirector::sharedDirector()->getVisibleSize();
    
    CCLabelTTF *spinLabel = CCLabelTTF::create("Spin", "Helvetica", 32);
    CCMenuItemLabel *menuSpinLabel = CCMenuItemLabel::create(spinLabel, this, menu_selector(HelloWorld::spinPicker));
    
    CCLabelTTF *spinSlowLabel = CCLabelTTF::create("Spin Slow", "Helvetica", 32);
    CCMenuItemLabel *menuSpinSlowLabel = CCMenuItemLabel::create(spinSlowLabel, this, menu_selector(HelloWorld::spinPickerSlow));
    
    CCLabelTTF *spinEaseLabel = CCLabelTTF::create("Spin Ease", "Helvetica", 32);
    CCMenuItemLabel *menuSpinEaseLabel = CCMenuItemLabel::create(spinEaseLabel, this, menu_selector(HelloWorld::spinPickerEase));

    
    CCMenu *mainMenu = CCMenu::create(menuSpinLabel, menuSpinSlowLabel, menuSpinEaseLabel, NULL);
    mainMenu->alignItemsVerticallyWithPadding(screenSize.height * 0.059f);
    mainMenu->setPosition(ccp(screenSize.width * 0.85f, screenSize.height/2.0f));
    this->addChild(mainMenu, 0, 0);
}

void HelloWorld::spinPicker()
{
    int stopRow0 = arc4random() % 10;
    int stopRow1 = arc4random() % 10;
    int stopRow2 = arc4random() % 10;
    
//    pickerView->spinComponent(0, 15, 1, 10, stopRow0);
//    pickerView->spinComponent(1, 15, 1, 10, stopRow1);
//    pickerView->spinComponent(2, 15, 1, 10, stopRow2);
    pickerView->spinComonentForever(0, 10, 1);
    pickerView->spinComonentForever(1, 10, 2);
    pickerView->spinComonentForever(2, 10, 3);
    
    feedbackLabel0->setString("Component 0 Spinning");
    feedbackLabel1->setString("Component 1 Spinning");
    feedbackLabel2->setString("Component 2 Spinning");
    
    feedbackLabel0->setColor(ccc3(255, 0, 0));
    feedbackLabel1->setColor(ccc3(255, 0, 0));
    feedbackLabel2->setColor(ccc3(255, 0, 0));
    

}

void HelloWorld::spinPickerSlow()
{
    int stopRow0 = arc4random() % 10;
    int stopRow1 = arc4random() % 10;
    int stopRow2 = arc4random() % 10;
    
    pickerView->spinComponent(0, 10, 1, 1, stopRow0);
    pickerView->spinComponent(1, 10, 1, 1, stopRow1);
    pickerView->spinComponent(2, 10, 1, 1, stopRow2);
    
    feedbackLabel0->setString("Component 0 Spinning");
    feedbackLabel1->setString("Component 1 Spinning");
    feedbackLabel2->setString("Component 2 Spinning");
    
    feedbackLabel0->setColor(ccc3(255, 0, 0));
    feedbackLabel1->setColor(ccc3(255, 0, 0));
    feedbackLabel2->setColor(ccc3(255, 0, 0));
}

void HelloWorld::spinPickerEase()
{
    int stopRow0 = arc4random() % 10;
    int stopRow1 = arc4random() % 10;
    int stopRow2 = arc4random() % 10;
    
    pickerView->spinComponent(0, 10, 1, 2, stopRow0);
    pickerView->spinComponent(1, 10, 2, 2, stopRow1);
    pickerView->spinComponent(2, 10, 3, 2, stopRow2);
    
    feedbackLabel0->setString("Component 0 Spinning");
    feedbackLabel1->setString("Component 1 Spinning");
    feedbackLabel2->setString("Component 2 Spinning");
    
    feedbackLabel0->setColor(ccc3(255, 0, 0));
    feedbackLabel1->setColor(ccc3(255, 0, 0));
    feedbackLabel2->setColor(ccc3(255, 0, 0));
}

int HelloWorld::numberOfComponentsInPickerView(CCPickerView *pickerView)
{
    return 3;
}

int HelloWorld::numberOfRowsInComponent(CCPickerView *pickerView, int component)
{
    return 10;
}

float HelloWorld::rowHeightForComponent(CCPickerView *pickerView, int component)
{
    return 32;
}

float HelloWorld::rowWidthForComponent(CCPickerView *pickerView, int component)
{
    return 54;
}

CCNode *HelloWorld::nodeForRow(CCPickerView *pickerView, int row, int component)
{
    CCLabelBMFont *label;
    if(test)
    {
        label = CCLabelBMFont::create(test, "bitmapFont.fnt");
    }
    else
    {
        CCString *str = CCString::createWithFormat("%d", row);
        label = CCLabelBMFont::create(str->getCString(), "bitmapFont.fnt");
    }
    return label;
}

void HelloWorld::didSelectRow(CCPickerView *pickerView, int row, int component)
{
    CCLOG("didSelect row = %d, component = %d", row, component);
}

float HelloWorld::spaceBetweenComponents(CCPickerView *pickerView)
{
    return 10;
}

CCNode *HelloWorld::overlayImage(CCPickerView *pickerView)
{
    CCSprite *sprite = CCSprite::create("转盘.png");
    return sprite;
}

void HelloWorld::onDoneSpinning(CCPickerView *pickerView, int component)
{
    switch (component) {
        case 0: {
            feedbackLabel0->setString("Component 0 Stopped");
            feedbackLabel0->setColor(ccc3(255, 255, 255));
            break;
        }
        case 1: {
            feedbackLabel1->setString("Component 1 Stopped");
            feedbackLabel1->setColor(ccc3(255, 255, 255));
            break;
        }
        case 2: {
            feedbackLabel2->setString("Component 2 Stopped");
            feedbackLabel2->setColor(ccc3(255, 255, 255));
            break;
        }
        default:
            break;
    }
    CCLOG("Component %d stopped spinning.", component);
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
