#include "GamesScence.h"

Scene* GamesScence::scene()
{
    Scene *scene       = Scene::create();
    GamesScence *layer = GamesScence::create();

    scene->addChild(layer);

    return scene;
}

bool GamesScence::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	m_blockSize      = 20.0f;
	m_isGgameRunning = false;
	m_next3Tetrominos.clear();
	
	Widget* pRootWidget = cocostudio::GUIReader::getInstance()->widgetFromJsonFile("TetrisUi/TetrisUi.ExportJson");
	this->addChild(pRootWidget,0);
	
	//最高分显示
	m_highestLabel = (TextAtlas*)Helper::seekWidgetByName(pRootWidget,"atlHighest");
	int highest    = UserDefault::getInstance()->getIntegerForKey("TheHighestScore",0);
	char strHighest[20];
	sprintf(strHighest,"%d",highest);
	m_highestLabel->setString(strHighest);
	
	//分数显示
	m_scoreCount.setNumberLabel((TextAtlas*)Helper::seekWidgetByName(pRootWidget,"atlScore"));

	//提示消除了多少行
	m_clearCount.setNumberLabel((TextAtlas*)Helper::seekWidgetByName(pRootWidget,"atlNumClear"));

	//菜单按钮
	m_btMenu = (Button*)Helper::seekWidgetByName(pRootWidget,"btMenu");
	m_btMenu->addTouchEventListener(CC_CALLBACK_2(GamesScence::btMenuCallback,this));

	//底板的纵横线
	m_imgFrame = (ImageView*)Helper::seekWidgetByName(pRootWidget,"imgFrame");

	//下一个方块的提示
	ImageView* nextTip = (ImageView*)Helper::seekWidgetByName(pRootWidget,"imgTipBoard");
	m_nextTipPos  = nextTip->getPosition();
	m_nextTipSize = nextTip->getContentSize();

	//游戏菜单
	m_panelManager.setMenuPanel((Layout*)Helper::seekWidgetByName(pRootWidget,"layMenu"));
	m_panelManager.setMenuPanelVisible(false,true);
	((Button*)Helper::seekWidgetByName(pRootWidget,"btStart"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btStartCallback,this));
	((Button*)Helper::seekWidgetByName(pRootWidget,"btContinue"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btContinueCallback,this));
	((Button*)Helper::seekWidgetByName(pRootWidget,"btRankList"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btRankListCallback,this));
	((Button*)Helper::seekWidgetByName(pRootWidget,"btHelp"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btHelpCallback,this));
	((Button*)Helper::seekWidgetByName(pRootWidget,"btLeave"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btLeaveCallback,this));
	
	//GameOver菜单
	m_panelManager.setGameOverPanel((Layout*)Helper::seekWidgetByName(pRootWidget,"layGameOver"));
	m_panelManager.setGameOverPanelVisible(false);
	((Button*)Helper::seekWidgetByName(pRootWidget,"btRestart"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btRestartCallback,this));
	
	//上传分数面板
	m_panelManager.setUploadScorePanel((Layout*)Helper::seekWidgetByName(pRootWidget,"layUploadScore"));
	m_panelManager.setUploadScorePanelVisible(0,false);
	m_nickNameInput = (TextField*)Helper::seekWidgetByName(pRootWidget,"tfNickName");
	((Button*)Helper::seekWidgetByName(pRootWidget,"btConfirmation"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btConfirmationCallback,this));
	((Button*)Helper::seekWidgetByName(pRootWidget,"btCancel"))->addTouchEventListener(CC_CALLBACK_2(GamesScence::btCancelCallback,this));
	
	//排行榜（只是显示用,与服务器的交互放m_list里实现）
	ListView* uilist = (ListView*)Helper::seekWidgetByName(pRootWidget,"listRankList");
	m_panelManager.setRankList(uilist);
	m_panelManager.setRankListVisible(false);

	////真正与服务器交互的排行榜功能类
	m_list = RankList::create(uilist);
	this->addChild(m_list);
	

	//游戏底板
	Node* pFrame = Helper::seekWidgetByName(pRootWidget,"imgFrame");
	Size frameSize = pFrame->getContentSize();

	m_bgBpard = BackgroundBoard::create(m_blockSize);
	m_bgBpard->setPosition(pFrame->getPosition());
	m_bgBpard->setScaleX(frameSize.width/BACKGROUND_COL/m_blockSize);
	m_bgBpard->setScaleY(frameSize.height/BACKGROUND_ROW/m_blockSize);
	
	m_bgBpard->setDropDelayTime(0.5f);
	m_bgBpard->setClearLineListener(this,clearLine_selector(GamesScence::onAddScore));
	m_bgBpard->setNextBlockListener(this,nextBlock_selector(GamesScence::onNextBlock));
	m_bgBpard->setGameOverListener(this,gameOver_selector(GamesScence::onGameOver));
	Helper::seekWidgetByName(pRootWidget,"root")->addChild(m_bgBpard,0);
	
    return true;
}

void GamesScence::btStartCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		startGame();
	}
}

void GamesScence::btRestartCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		startGame();
	}
}

void GamesScence::btMenuCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		m_bgBpard->pasueDrop();

		m_panelManager.setMenuPanelVisible(m_isGgameRunning,true);

		m_bgBpard->setVisible(true);

		m_imgFrame->setVisible(true);
	}
}

void GamesScence::btContinueCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		m_bgBpard->continueDrop();
		m_isGgameRunning = true;
		m_panelManager.setMenuPanelVisible(m_isGgameRunning,false);
	}
}

void GamesScence::btRankListCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		m_list->downloadRankList();
		m_panelManager.setRankListVisible(true);
		m_bgBpard->setVisible(false);
		m_imgFrame->setVisible(false);
	}
}

void GamesScence::btHelpCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		HttpTool::getInstance()->getPosition(12,this,getPosition_selector(GamesScence::getPositionResponse));
	}
}
void GamesScence::btConfirmationCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		std::string nickName = m_nickNameInput->getStringValue();
		HttpTool::getInstance()->uploadScore(nickName.c_str(),m_scoreCount.getNumber(),this,uploadScore_selector(GamesScence::uploadScoreResponse));
	}
}

void GamesScence::btCancelCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		m_panelManager.setGameOverPanelVisible(true);
	}
}

void GamesScence::uploadScoreResponse(bool b)
{
	if(b)
	{
		MessageBox("OK!","OK");
		m_panelManager.setGameOverPanelVisible(true);
	}
	else
		MessageBox("Failure!","Failure");
}

void GamesScence::btLeaveCallback(Ref* pSender,Widget::TouchEventType type)
{
	if(type == Widget::TouchEventType::ENDED)
	{
		network::HttpClient::getInstance()->destroyInstance();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
			MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
			return;
		#endif

			Director::getInstance()->end();

		#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			exit(0);
		#endif
	}
}

void GamesScence::onAddScore(int numLineCleared)
{
	if(numLineCleared>0)
	{
		m_scoreCount.increaseBy((1<<numLineCleared));
		m_clearCount.increaseBy(numLineCleared);

		m_dropDelayTime = 0.5f - m_clearLineCount/10*0.015f;
		m_bgBpard->setDropDelayTime(m_dropDelayTime);
		
	}
}

void GamesScence::onNextBlock(int* next3Blocks)
{
	if(m_next3Tetrominos.size() == 0)
	{
		Point pos = m_nextTipPos;
		for(int i = 0 ; i<3 ; ++i)
		{
			Tetromino* t = Tetromino::create(next3Blocks[i],m_blockSize,0,this,false);

			float targetSize    = m_nextTipSize.width;
			Point targetPos   = pos;
			TetrominoSize tSize = t->getTetrominoSize();

			//位置偏移到中间
			targetPos.x-=((targetSize-tSize._col*targetSize/4.0f)/2.0f);
			targetPos.y+=((targetSize-tSize._row*targetSize/4.0f)/2.0f);

			t->setCurBlockNodePos(targetPos);
			t->setCurBlockNodeScale(targetSize/4.0f/m_blockSize);
	
			pos.y-=m_nextTipSize.height;

			m_next3Tetrominos.push_back(t);
		}
		
	}
	else
	{
		//从cocos渲染中消除下一个的提示
		std::list<Tetromino*>::iterator i = m_next3Tetrominos.begin();
		Tetromino::destory(&(*i));

		//之后的第二三的位置往上移动
		i++;
		(*i)->setCurBlockNodeAction(CCMoveBy::create(0.3f,Vec2(0.0f,m_nextTipSize.height)));

		i++;
		(*i)->setCurBlockNodeAction(CCMoveBy::create(0.3f,Vec2(0.0f,m_nextTipSize.height)));

		//从队列中消除下一个的提示
		m_next3Tetrominos.pop_front();

		//新增一个提示,凑成三个
		Point pos = m_nextTipPos;
		pos.y-=(2*m_nextTipSize.height);

		Tetromino* t = Tetromino::create(next3Blocks[2],m_blockSize,0,this,false);

		float targetSize  = m_nextTipSize.width;
		Point targetPos   = pos;
		TetrominoSize tSize = t->getTetrominoSize();

		//位置偏移到中间
		targetPos.x-=((targetSize-tSize._col*targetSize/4.0f)/2.0f);
		targetPos.y+=((targetSize-tSize._row*targetSize/4.0f)/2.0f);

		t->setCurBlockNodePos(targetPos);
		t->setCurBlockNodeScale(targetSize/4.0f/m_blockSize);
		
		t->setCurBlockNodeVisible(false);
		t->setCurBlockNodeAction(CCSequence::create(
			CCDelayTime::create(0.3f),
			CCShow::create(),
			NULL));

		m_next3Tetrominos.push_back(t);
	}
}

void GamesScence::onGameOver()
{
	m_isGgameRunning = false;
	m_panelManager.setGameOverPanelVisible(true);

	int highestScore = UserDefault::getInstance()->getIntegerForKey("TheHighestScore",0);

	int score = m_scoreCount.getNumber();
	if(highestScore<score)
	{
		highestScore = score;
		UserDefault::getInstance()->setIntegerForKey("TheHighestScore",highestScore);
	}

	char strHighest[20];
	sprintf(strHighest,"%d",highestScore);
	m_highestLabel->setString(strHighest);
	HttpTool::getInstance()->getPosition(score,this,getPosition_selector(GamesScence::getPositionResponse));
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

	m_bgBpard->start();
	m_bgBpard->setDropDelayTime(m_dropDelayTime);
	m_isGgameRunning = true;
	m_panelManager.setGameOverPanelVisible(false);
	m_panelManager.setMenuPanelVisible(m_isGgameRunning,false);

	m_scoreCount.setNumber(0);
	m_clearCount.setNumber(0);
}