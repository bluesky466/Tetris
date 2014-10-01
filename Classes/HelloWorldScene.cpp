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
	
	CCSprite* sprite= CCSprite::create("HelloWorld.png");
    sprite->setPosition(ccp(visibleSize.width/2, visibleSize.height/2));
    this->addChild(sprite, 0);

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

	bg = BackgroundBoard::create(20.0f,"block.png");
	bg->setPosition(visibleSize.width + origin.x, 0.0f);
	bg->setDropDur(0.5f);
	bg->start();
	this->addChild(bg);


    return true;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	
}

void HelloWorld::menuCloseCallback2(CCObject* pSender)
{
	
}
