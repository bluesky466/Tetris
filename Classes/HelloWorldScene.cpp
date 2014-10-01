#include "HelloWorldScene.h"
#include "FragmentEffect.h"


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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

	Matrix44 m0 = {
		0.8f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m0);

	Matrix44 m1 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.8f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m1);

	Matrix44 m2 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m2);

	Matrix44 m3 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.8f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m3);

	Matrix44 m4 = {
		0.8f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m4);

	Matrix44 m5 = {
		1.0f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.4f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m5);

	Matrix44 m6 = {
		0.6f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.7f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.7f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.6f
	};
	FragmentEffect::getInstance()->addEffectMatrix(m6);

	TouchGroup* ul =TouchGroup::create();
	ul->addWidget(GUIReader::shareReader()->widgetFromJsonFile("TetrisUi/TetrisUi.ExportJson"));
	this->addChild(ul,0);


	m_scoreLabel = (UILabelAtlas*)ul->getWidgetByName("altScore");
	m_scoreLabel->setStringValue("0");
	m_score =0;

	m_btStart = (UIButton*)ul->getWidgetByName("btStart");
	m_btStart->addTouchEventListener(this,toucheventselector(HelloWorld::startCallback));

	bg = BackgroundBoard::create(20.0f,"block.png");
	UIWidget* pFrame = ul->getWidgetByName("imgFrame");
	CCSize frameSize = pFrame->getContentSize();
	bg->setPosition(pFrame->getPosition());
	bg->setScaleX(frameSize.width/12/20.0f);
	bg->setScaleY(frameSize.height/20/20.0f);
	ul->getWidgetByName("root")->addNode(bg,1);
	bg->setDropDur(0.5f);
	bg->start();
	bg->setClearLineListener(this,clearLine_selector(HelloWorld::addScore));


    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	
}

void HelloWorld::menuCloseCallback2(CCObject* pSender)
{
	
}

void HelloWorld::startCallback(CCObject* pObject, TouchEventType e)
{
	if(e == TOUCH_EVENT_ENDED)
	{
		if(bg->isPause())
		{
			bg->continueDrop();
			m_btStart->setTitleText("pasue");
		}
		else
		{
			bg->pasueDrop();
			m_btStart->setTitleText("continue");
		}
	}
}

void HelloWorld::addScore(int numLineCleared)
{
	if(numLineCleared>0)
	{
		char str[10];
		m_score+=((1<<numLineCleared)*100);
		sprintf(str,"%d",m_score);
		m_scoreLabel->setStringValue(str);
	}

	
}