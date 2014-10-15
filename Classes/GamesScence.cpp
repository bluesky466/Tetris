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

	m_score          = 0;
	m_blockSize      = 20.0f;
	m_isGgameRunning = false;
	m_next3Tetrominos.clear();

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

	//底板的纵横线
	m_imgFrame = (UIImageView*)m_uiLayer->getWidgetByName("imgFrame");

	//提示消除了多少行
	m_numCLearLabel = (UILabelAtlas*)m_uiLayer->getWidgetByName("atlNumClear");

	//下一个方块的提示
	UIImageView* nextTip = (UIImageView*)m_uiLayer->getWidgetByName("imgTipBoard");
	m_nextTipPos  = nextTip->getPosition();
	m_nextTipSize = nextTip->getContentSize();

	//游戏菜单
	m_panelManager.setMenuPanel((UIPanel*)m_uiLayer->getWidgetByName("layMenu"));
	m_panelManager.setMenuPanelVisible(false,true);
	((UIButton*)m_uiLayer->getWidgetByName("btStart"))->addTouchEventListener(this,toucheventselector(GamesScence::btStartCallback));
	((UIButton*)m_uiLayer->getWidgetByName("btContinue"))->addTouchEventListener(this,toucheventselector(GamesScence::btContinueCallback));
	((UIButton*)m_uiLayer->getWidgetByName("btRankList"))->addTouchEventListener(this,toucheventselector(GamesScence::btRankListCallback));
	((UIButton*)m_uiLayer->getWidgetByName("btHelp"))->addTouchEventListener(this,toucheventselector(GamesScence::btHelpCallback));
	((UIButton*)m_uiLayer->getWidgetByName("btLeave"))->addTouchEventListener(this,toucheventselector(GamesScence::btLeaveCallback));

	//GameOver菜单
	m_panelManager.setGameOverPanel((UIPanel*)m_uiLayer->getWidgetByName("layGameOver"));
	m_panelManager.setGameOverPanelVisible(false);
	((UIButton*)m_uiLayer->getWidgetByName("btRestart"))->addTouchEventListener(this,toucheventselector(GamesScence::btRestartCallback));
	
	//上传分数面板
	m_panelManager.setUploadScorePanel((UIPanel*)m_uiLayer->getWidgetByName("layUploadScore"));
	m_panelManager.setUploadScorePanelVisible(0,false);
	m_nickNameInput    = (UITextField*)m_uiLayer->getWidgetByName("tfNickName");
	((UIButton*)m_uiLayer->getWidgetByName("btConfirmation"))->addTouchEventListener(this,toucheventselector(GamesScence::btConfirmationCallback));
	((UIButton*)m_uiLayer->getWidgetByName("btCancel"))->addTouchEventListener(this,toucheventselector(GamesScence::btCancelCallback));
	
	//排行榜（只是显示用,与服务器的交互放m_list里实现）
	UIListView* uilist = (UIListView*)m_uiLayer->getWidgetByName("listRankList");
	m_panelManager.setRankList(uilist);
	m_panelManager.setRankListVisible(false);

	//真正与服务器交互的排行榜功能类
	m_list = RankList::create(uilist);
	this->addChild(m_list);
	

	//游戏底板
	UIWidget* pFrame = m_uiLayer->getWidgetByName("imgFrame");
	CCSize frameSize = pFrame->getContentSize();

	m_bgBpard = BackgroundBoard::create(m_blockSize,"block.png");
	m_bgBpard->setPosition(pFrame->getPosition());
	m_bgBpard->setScaleX(frameSize.width/BACKGROUND_COL/m_blockSize);
	m_bgBpard->setScaleY(frameSize.height/BACKGROUND_ROW/m_blockSize);
	
	m_bgBpard->setDropDelayTime(0.5f);
	m_bgBpard->setClearLineListener(this,clearLine_selector(GamesScence::onAddScore));
	m_bgBpard->setNextBlockListener(this,nextBlock_selector(GamesScence::onNextBlock));
	m_bgBpard->setGameOverListener(this,gameOver_selector(GamesScence::onGameOver));
	m_uiLayer->getWidgetByName("root")->addNode(m_bgBpard,0);
	
    return true;
}

void GamesScence::setEffectMatrix()
{
	//效果矩阵的设置,苦力活
	float norA    = 0.0f;
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

void GamesScence::btStartCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		startGame();
	}
}

void GamesScence::btRestartCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		startGame();
	}
}

void GamesScence::btMenuCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_bgBpard->pasueDrop();

		m_panelManager.setMenuPanelVisible(m_isGgameRunning,true);

		m_bgBpard->setVisible(true);

		m_imgFrame->setVisible(true);
	}
}

void GamesScence::btContinueCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_bgBpard->continueDrop();
		m_isGgameRunning = true;
		m_panelManager.setMenuPanelVisible(m_isGgameRunning,false);
	}
}

void GamesScence::btRankListCallback(CCObject* pSender,TouchEventType type)
{
	if(type == TOUCH_EVENT_ENDED)
	{
		m_list->downloadRankList();
		m_panelManager.setRankListVisible(true);
		m_bgBpard->setVisible(false);
		m_imgFrame->setVisible(false);
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
		m_panelManager.setUploadScorePanelVisible(0,false);
	}
}

void GamesScence::uploadScoreResponse(bool b)
{
	if(b)
	{
		CCMessageBox("OK!","OK");
		m_panelManager.setGameOverPanelVisible(true);
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

		m_clearLineCount += numLineCleared;
		sprintf(str,"%d",m_clearLineCount);
		m_numCLearLabel->setStringValue(str);

		m_dropDelayTime = 0.2f - m_clearLineCount/10*0.015f;
		m_bgBpard->setDropDelayTime(m_dropDelayTime);
		
	}
}

void GamesScence::onNextBlock(int* next3Blocks)
{
	if(m_next3Tetrominos.size() == 0)
	{
		CCPoint pos = m_nextTipPos;
		for(int i = 0 ; i<3 ; ++i)
		{
			Tetromino* t = Tetromino::create(next3Blocks[i],m_blockSize,"block.png");

			float targetSize    = m_nextTipSize.width;
			CCPoint targetPos   = pos;
			TetrominoSize tSize = t->getTetrominoSize();

			//位置偏移到中间
			targetPos.x-=((targetSize-tSize._col*targetSize/4.0f)/2.0f);
			targetPos.y+=((targetSize-tSize._row*targetSize/4.0f)/2.0f);

			t->setPosition(targetPos);
			t->setScale(targetSize/4.0f/m_blockSize);
	
			m_uiLayer->getWidgetByName("root")->addNode(t,6);

			pos.y-=m_nextTipSize.height;

			m_next3Tetrominos.push_back(t);
		}
		
	}
	else
	{
		//从cocos渲染中消除下一个的提示
		std::list<Tetromino*>::iterator i = m_next3Tetrominos.begin();
		m_uiLayer->getWidgetByName("root")->removeChild(*i);

		//之后的第二三的位置往上移动
		i++;
		(*i)->runAction(CCMoveBy::create(0.2f,ccp(0.0f,m_nextTipSize.height)));

		i++;
		(*i)->runAction(CCMoveBy::create(0.2f,ccp(0.0f,m_nextTipSize.height)));

		//从队列中消除下一个的提示
		m_next3Tetrominos.pop_front();

		//新增一个提示,凑成三个
		CCPoint pos = m_nextTipPos;
		pos.y-=(2*m_nextTipSize.height);

		Tetromino* t = Tetromino::create(next3Blocks[2],m_blockSize,"block.png");

		float targetSize    = m_nextTipSize.width;
		CCPoint targetPos   = pos;
		TetrominoSize tSize = t->getTetrominoSize();

		//位置偏移到中间
		targetPos.x-=((targetSize-tSize._col*targetSize/4.0f)/2.0f);
		targetPos.y+=((targetSize-tSize._row*targetSize/4.0f)/2.0f);

		t->setPosition(targetPos);
		t->setScale(targetSize/4.0f/m_blockSize);
		
		t->setVisible(false);
		t->runAction(CCSequence::create(
			CCDelayTime::create(0.2f),
			CCShow::create(),
			NULL));

		m_uiLayer->getWidgetByName("root")->addNode(t,6);
		m_next3Tetrominos.push_back(t);
	}
}

void GamesScence::onGameOver()
{
	m_isGgameRunning = false;
	m_panelManager.setGameOverPanelVisible(true);

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

void GamesScence::getPositionResponse(int position)
{
	if(position>0 && position<=100)
		m_panelManager.setUploadScorePanelVisible(position,true);
}

void GamesScence::startGame()
{
	m_dropDelayTime  = 0.5f;
	m_clearLineCount = 0;
	m_score          = 0;

	m_bgBpard->start();
	m_bgBpard->setDropDelayTime(m_dropDelayTime);
	m_isGgameRunning = true;
	m_panelManager.setGameOverPanelVisible(false);
	m_panelManager.setMenuPanelVisible(m_isGgameRunning,false);

	m_scoreLabel->setStringValue("0");
	m_numCLearLabel->setStringValue("0");

}