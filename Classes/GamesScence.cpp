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

	m_uiLayer = TouchGroup::create();
	m_uiLayer->addWidget(GUIReader::shareReader()->widgetFromJsonFile("TetrisUi/TetrisUi.ExportJson"));
	this->addChild(m_uiLayer,0);

	//最高分显示
	m_highestLabel = (UILabelAtlas*)m_uiLayer->getWidgetByName("atlHighest");
	int highest = CCUserDefault::sharedUserDefault()->getIntegerForKey("TheHighestScore",0);
	char strHighest[20];
	sprintf(strHighest,"%d",highest);
	m_highestLabel->setStringValue(strHighest);
	
	//分数显示
	m_scoreLabel = (UILabelAtlas*)m_uiLayer->getWidgetByName("atlScore");
	m_scoreLabel->setStringValue("0");
	
	//菜单按钮
	m_btMenu = (UIButton*)m_uiLayer->getWidgetByName("btMenu");
	m_btMenu->addTouchEventListener(this,toucheventselector(GamesScence::btMenuCallback));
	m_btMenu->setTouchEnabled(false);
	m_btMenu->setVisible(false);

	//开始按钮
	m_btStart = (UIButton*)m_uiLayer->getWidgetByName("btStart");
	m_btStart->addTouchEventListener(this,toucheventselector(GamesScence::btStartCallback));

	//继续按钮
	m_btContinue = (UIButton*)m_uiLayer->getWidgetByName("btContinue");
	m_btContinue->addTouchEventListener(this,toucheventselector(GamesScence::btContinueCallback));
	m_btContinue->setTouchEnabled(false);
	m_btContinue->setVisible(false);

	//排行榜按钮
	m_btStart = (UIButton*)m_uiLayer->getWidgetByName("btRankList");
	m_btStart->addTouchEventListener(this,toucheventselector(GamesScence::btRankListCallback));

	//帮助按钮
	m_btStart = (UIButton*)m_uiLayer->getWidgetByName("btHelp");
	m_btStart->addTouchEventListener(this,toucheventselector(GamesScence::btHelpCallback));

	//离开游戏按钮
	m_btStart = (UIButton*)m_uiLayer->getWidgetByName("btLeave");
	m_btStart->addTouchEventListener(this,toucheventselector(GamesScence::btLeaveCallback));
	
	//排行榜
	m_listRankList = (UIListView*)m_uiLayer->getWidgetByName("listRankList");
	m_listRankList->setVisible(false);
	m_listRankList->setTouchEnabled(false);
	m_list = RankList::create(m_listRankList);
	this->addChild(m_list);

	//游戏菜单
	m_menuPanel = (UIPanel*)m_uiLayer->getWidgetByName("layMenu");

	//底板的纵横线
	m_imgFrame = (UIImageView*)m_uiLayer->getWidgetByName("imgFrame");

	//GameOver菜单
	m_gamrOverPanel = (UIPanel*)m_uiLayer->getWidgetByName("layGameOver");
	UIButton* restart = (UIButton*)m_uiLayer->getWidgetByName("btRestart");
	restart->addTouchEventListener(this,toucheventselector(GamesScence::btRestartCallback));
	setGameOverPanelVisible(false);

	//上传分数面板
	m_uploadScorePanel = (UIPanel*)m_uiLayer->getWidgetByName("layUploadScore");
	m_nickNameInput = (UITextField*)m_uiLayer->getWidgetByName("tfNickName");
	UIButton* btConfirmation = (UIButton*)m_uiLayer->getWidgetByName("btConfirmation");
	btConfirmation->addTouchEventListener(this,toucheventselector(GamesScence::btConfirmationCallback));
	UIButton* btCancel = (UIButton*)m_uiLayer->getWidgetByName("btCancel");
	btCancel->addTouchEventListener(this,toucheventselector(GamesScence::btCancelCallback));
	setUploadScorePanelVisible(0,false);

	//游戏底板
	UIWidget* pFrame = m_uiLayer->getWidgetByName("imgFrame");
	CCSize frameSize = pFrame->getContentSize();

	m_bgBpard = BackgroundBoard::create(m_blockSize,"block.png");
	m_bgBpard->setPosition(pFrame->getPosition());
	m_bgBpard->setScaleX(frameSize.width/BACKGROUND_COL/m_blockSize);
	m_bgBpard->setScaleY(frameSize.height/BACKGROUND_ROW/m_blockSize);
	
	m_bgBpard->setDropDelayTime(0.5f);
	m_bgBpard->setClearLineListener(this,clearLine_selector(GamesScence::onAddScore));
	m_bgBpard->setGameOverListener(this,gameOver_selector(GamesScence::onGameOver));

	m_uiLayer->getWidgetByName("root")->addNode(m_bgBpard,0);
	
    return true;
}

void GamesScence::setEffectMatrix()
{
	//效果矩阵的设置,苦力活
	float norA    = 0.05f;
	float targetA = 1.0f;

	Matrix44 m0 = {
		0.4f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.1f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.1f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m0);

	Matrix44 m1 = {
		0.1f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.4f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.1f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m1);

	Matrix44 m2 = {
		0.1f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.1f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.4f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m2);

	Matrix44 m3 = {
		0.1f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.4f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.4f, 0.0f,
		0.0f, 0.0f, 0.0f, norA
	};
	FragmentEffect::getInstance()->addEffectMatrix(m3);

	Matrix44 m4 = {
		0.4f, 0.0f, 0.0f, 0.0f,						
		0.0f, 0.1f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.4f, 0.0f,
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

void GamesScence::btStartCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_bgBpard->start();
		m_iGgameRunning = true;
		setMenuVisible(false);

		m_score = 0;
		m_scoreLabel->setStringValue("0");
	}
}

void GamesScence::btRestartCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_bgBpard->start();
		m_iGgameRunning = true;
		setGameOverPanelVisible(false);

		m_score = 0;
		m_scoreLabel->setStringValue("0");
	}
}

void GamesScence::btMenuCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_bgBpard->pasueDrop();
		setMenuVisible(true);
		setGameOverPanelVisible(false);

		m_listRankList->setVisible(false);
		m_listRankList->setTouchEnabled(false);
		m_imgFrame->setVisible(true);
		m_bgBpard->setVisible(true);
	}
}

void GamesScence::btContinueCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_bgBpard->continueDrop();
		m_iGgameRunning = true;
		setMenuVisible(false);
	}
}

void GamesScence::btRankListCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_list->downloadRankList();
		m_listRankList->setVisible(true);
		m_listRankList->setTouchEnabled(true);
		m_imgFrame->setVisible(false);
		m_bgBpard->setVisible(false);
		setMenuVisible(false);
	}
}

void GamesScence::btHelpCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		
	}
}
void GamesScence::btConfirmationCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		std::string nickName = m_nickNameInput->getStringValue();
		HttpTool::getInstance()->uploadScore(nickName.c_str(),m_score,this,uploadScore_selector(GamesScence::uploadScoreResponse));
	}
}

void GamesScence::btCancelCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		setUploadScorePanelVisible(0,false);
	}
}

void GamesScence::uploadScoreResponse(bool b)
{
	if(b)
	{
		CCMessageBox("OK!","OK");
		setUploadScorePanelVisible(0,false);
	}
	else
		CCMessageBox("Failure!","Failure");
}

void GamesScence::btLeaveCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
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
	setGameOverPanelVisible(true);

	int highestScore = CCUserDefault::sharedUserDefault()->getIntegerForKey("TheHighestScore",0);

	if(highestScore<m_score)
	{
		highestScore = m_score;
		CCUserDefault::sharedUserDefault()->setIntegerForKey("TheHighestScore",highestScore);
	}

	char strHighest[20];
	sprintf(strHighest,"%d",highestScore);
	m_highestLabel->setStringValue(strHighest);
	HttpTool::getInstance()->getPosition(m_score,this,getPosition_selector(GamesScence::getPositionResponse));
}

void GamesScence::setGameOverPanelVisible(bool bVisible)
{
	m_gamrOverPanel->setVisible(bVisible);
	for(int i =0 ; i<2 ; ++i)
	{
		UIWidget* pChild = (UIWidget*)m_gamrOverPanel->getChildByTag(i);
		pChild->setVisible(bVisible);
		pChild->setTouchEnabled(bVisible);
	}
}

void GamesScence::setUploadScorePanelVisible(int position,bool bVisible)
{
	m_uploadScorePanel->setVisible(bVisible);
	for(int i =0 ; i<3 ; ++i)
	{
		UIWidget* pChild = (UIWidget*)m_uploadScorePanel->getChildByTag(i);
		pChild->setVisible(bVisible);
		pChild->setTouchEnabled(bVisible);
	}

	if(bVisible)
	{
		char str[20];
		sprintf(str,"%d",position);
		((UILabelAtlas*)m_uploadScorePanel->getChildByTag(0))->setStringValue(str);
	}
}

void GamesScence::setMenuVisible(bool bVisible)
{
	for(int i = 0 ; i<5 ; ++i)
	{
		UIWidget* pChild = (UIWidget*)m_menuPanel->getChildByTag(i);
		pChild->setVisible(bVisible);
		pChild->setTouchEnabled(bVisible);
	}

	m_btMenu->setTouchEnabled(!bVisible);
	m_btMenu->setVisible(!bVisible);

	if(bVisible && m_iGgameRunning)
	{
		UIWidget* pChild = (UIWidget*)m_menuPanel->getChildByTag(0);
		pChild->setVisible(false);
		pChild->setTouchEnabled(false);

		pChild = (UIWidget*)m_menuPanel->getChildByTag(1);
		pChild->setVisible(true);
		pChild->setTouchEnabled(true);
	}
	else if(bVisible && !m_iGgameRunning)
	{
		UIWidget* pChild = (UIWidget*)m_menuPanel->getChildByTag(0);
		pChild->setVisible(true);
		pChild->setTouchEnabled(true);

		pChild = (UIWidget*)m_menuPanel->getChildByTag(1);
		pChild->setVisible(false);
		pChild->setTouchEnabled(false);
	}
}

void GamesScence::getPositionResponse(int position)
{
	if(position>0 && position<=100)
		setUploadScorePanelVisible(position,true);
}