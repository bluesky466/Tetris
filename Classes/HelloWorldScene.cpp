#include "HelloWorldScene.h"
#include "FragmentEffect.h"


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

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	bg = BackgroundBoard::create(20.0f,"CloseNormal.png");
	bg->setPosition(visibleSize.width + origin.x, 0.0f);
	bg->setDropDur(0.5f);
	this->addChild(bg);

	Matrix44 m = {
		1.0f, 0.0f, 0.0f, 0.0f,						
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	//七种效果先这样设置
	for(int i = 0 ; i<7 ; i++)
		FragmentEffect::getInstance()->addEffectMatrix(m);
    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	
}

void HelloWorld::menuCloseCallback2(CCObject* pSender)
{
	
}
