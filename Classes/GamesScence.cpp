#include "GamesScence.h"

CCScene* GamesScence::scene()
{
    CCScene *scene     = CCScene::create();
    GamesScence *layer = GamesScence::create();

    scene->addChild(layer);

    return scene;
}

bool GamesScence::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

	m_score     = 0;
	m_blockSize = 20.0f;
	m_iGgameRunning = false;

	setEffectMatrix();

	TouchGroup* ul = TouchGroup::create();
	ul->addWidget(GUIReader::shareReader()->widgetFromJsonFile("TetrisUi/TetrisUi.ExportJson"));
	this->addChild(ul,0);

	//最高分显示
	m_highestLabel = (UILabelAtlas*)ul->getWidgetByName("atlHighest");
	int highest = CCUserDefault::sharedUserDefault()->getIntegerForKey("TheHighestScore",0);
	char strHighest[20];
	sprintf(strHighest,"%d",highest);
	m_highestLabel->setStringValue(strHighest);
	
	//分数显示
	m_scoreLabel = (UILabelAtlas*)ul->getWidgetByName("altScore");
	m_scoreLabel->setStringValue("0");
	

	//开始按钮
	m_btStart = (UIButton*)ul->getWidgetByName("btStart");
	m_btStart->addTouchEventListener(this,toucheventselector(GamesScence::startCallback));



	//游戏底板
	UIWidget* pFrame = ul->getWidgetByName("imgFrame");
	CCSize frameSize = pFrame->getContentSize();

	m_bgBpard = BackgroundBoard::create(m_blockSize,"block.png");
	m_bgBpard->setPosition(pFrame->getPosition());
	m_bgBpard->setScaleX(frameSize.width/BACKGROUND_COL/m_blockSize);
	m_bgBpard->setScaleY(frameSize.height/BACKGROUND_ROW/m_blockSize);
	
	m_bgBpard->setDropDelayTime(0.5f);
	m_bgBpard->setClearLineListener(this,clearLine_selector(GamesScence::onAddScore));
	m_bgBpard->setGameOverListener(this,gameOver_selector(GamesScence::onGameOver));

	ul->getWidgetByName("root")->addNode(m_bgBpard,1);

    return true;
}

void GamesScence::setEffectMatrix()
{
	//效果矩阵的设置,苦力活
	float norA    = 0.6f;
	float targetA = 0.1f;

	Matrix44 m0 = {
		0.8f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m0);

	Matrix44 m1 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.8f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m1);

	Matrix44 m2 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m2);

	Matrix44 m3 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.8f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m3);

	Matrix44 m4 = {
		0.8f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m4);

	Matrix44 m5 = {
		1.0f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.4f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m5);

	Matrix44 m6 = {
		0.6f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.7f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.7f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m6);

	Matrix44 m7 = {
		0.8f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m7);

	Matrix44 m8 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.8f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m8);

	Matrix44 m9 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m9);

	Matrix44 m10 = {
		0.2f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.8f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m10);

	Matrix44 m11 = {
		0.8f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.2f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.8f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m11);

	Matrix44 m12 = {
		1.0f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.4f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.2f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m12);

	Matrix44 m13 = {
		0.6f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.7f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.7f, 0.0f,
		0.0f, 0.0f, 0.0f, targetA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m13);
}

void GamesScence::closeCallback(CCObject* pSender)
{
	
}

void GamesScence::startCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		if(!m_iGgameRunning)
		{
			m_bgBpard->start();
			m_iGgameRunning = true;
			m_btStart->setTitleText("pasue");
			m_scoreLabel->setStringValue("0");
		}
		else if(m_bgBpard->isPause())
		{
			m_bgBpard->continueDrop();
			m_btStart->setTitleText("pasue");
		}
		else
		{
			m_bgBpard->pasueDrop();
			m_btStart->setTitleText("continue");
		}
	}
}

void GamesScence::onAddScore(int numLineCleared)
{
	if(numLineCleared>0)
	{
		char str[10];
		m_score+=(1<<numLineCleared);
		sprintf(str,"%d",m_score);
		m_scoreLabel->setStringValue(str);
	}
}

void GamesScence::onGameOver()
{
	m_iGgameRunning = false;
	CCMessageBox("GameOver","GameOver");
	m_btStart->setTitleText("start");

	int highestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("TheHighestScore",0);

	if(highestScore<m_score)
	{
		highestScore = m_score;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("TheHighestScore",highestScore);
	}

	char strHighest[20];
	sprintf(strHighest,"%d",highestScore);
	m_highestLabel->setStringValue(strHighest);
}