#include "BackgroundBoard.h"

BackgroundBoard* BackgroundBoard::create(float blockSize,char* fnBlockTex)
{
	BackgroundBoard *pRet = new BackgroundBoard(); 

    if (pRet && pRet->init(blockSize,fnBlockTex)) 
    { 
        pRet->autorelease(); 
        return pRet; 
    } 
    else 
    { 
        delete pRet; 
        pRet = NULL; 
        return NULL; 
    } 
}

bool BackgroundBoard::init(float blockSize,char* fnBlockTex)
{
	if(!CCNode::init())
		return false;

	memset(m_bgInfo,0,BACKGROUND_ROW*sizeof(int));

	srand(time(0));

	m_isPause	     = false;
	m_fnBlockTex     = fnBlockTex;
	m_blockSize      = blockSize;
	m_actSensitivity = 10.0f;
	m_dropDur        = 0.1f;
	m_clearLineListener = 0;
	m_clearLineCallback = 0;
	m_gameOverListener  = 0;
	m_gameOverCallback  = 0;
	m_targetBlock       = 0;

	return true;
}

void BackgroundBoard::addNewTetromino()
{
	m_accDropDur = m_dropDur;

	m_curTetromino = Tetromino::create(rand()%7,m_blockSize,m_fnBlockTex);

	int col = m_curTetromino->getCol();
	int row = m_curTetromino->getRow();
	
	m_curTetromino->setPosition(-col*m_blockSize,row*m_blockSize);

	if(m_curTetromino->drop(m_bgInfo))
	{
		col = m_curTetromino->getCol();
		row = m_curTetromino->getRow();
		CCSequence* pSqe = CCSequence::create(CCDelayTime::create(m_accDropDur),
											  CCMoveBy::create(0.1f,ccp(0.0f,-m_blockSize)), 
											  CCCallFunc::create(this,callfunc_selector(BackgroundBoard::curTetrominoMove)),
											  NULL);

		m_curTetromino->runAction(pSqe);
		this->addChild(m_curTetromino);

		m_targetBlock = m_curTetromino->getTargetBlockNode(m_bgInfo,m_fnBlockTex);
		setTargetBlockPos();
		this->addChild(m_targetBlock);
	}
	else
	{
		m_curTetromino->release();
		m_curTetromino = 0;
		onGameOver();
	}
}

void BackgroundBoard::setTargetBlockPos()
{
	if(m_targetBlock)
	{
		int col = m_curTetromino->getCol();
		int row = m_curTetromino->getTargetRow();
		m_targetBlock->setPosition(ccp(-m_blockSize*col,m_blockSize*row));
	}
}

void BackgroundBoard::curTetrominoMove()
{
	if(m_curTetromino->drop(m_bgInfo))
	{
		int col = m_curTetromino->getCol();
		int row = m_curTetromino->getRow();
		CCSequence* pSqe = CCSequence::create(CCDelayTime::create(m_accDropDur),
											  CCMoveBy::create(0.1f,ccp(0.0f,-m_blockSize)), 
											  CCCallFunc::create(this,callfunc_selector(BackgroundBoard::curTetrominoMove)),
											  NULL);

		m_curTetromino->runAction(pSqe);
	}

	else
	{
		CCSequence* pSeq = CCSequence::create(CCDelayTime::create(m_accDropDur),
											  CCCallFunc::create(this,callfunc_selector(BackgroundBoard::setNextTetromino)),
											  NULL);
		this->runAction(pSeq);
		
	}
}

void BackgroundBoard::setNextTetromino()
{
	if(m_curTetromino == 0)
		return;

	if(m_curTetromino->drop(m_bgInfo))
	{
		int col = m_curTetromino->getCol();
		int row = m_curTetromino->getRow();
		CCSequence* pSqe = CCSequence::create(CCDelayTime::create(m_accDropDur),
											  CCMoveBy::create(0.1f,ccp(0.0f,-m_blockSize)), 
											  CCCallFunc::create(this,callfunc_selector(BackgroundBoard::curTetrominoMove)),
											  NULL);

		m_curTetromino->runAction(pSqe);
	}
	
	else if(!addToBg())
		onGameOver();

	else
		addNewTetromino();
			
}

bool BackgroundBoard::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bGameOver || !m_curTetromino)
		return false;

	CCPoint point = this->convertToNodeSpace(pTouch->getLocation());

	if(point.x>-12*m_blockSize && point.x<0.0f &&  
	   point.y>0.0f && point.y<20.0f*m_blockSize)
	{
		m_touchPos  = pTouch->getLocation();
		m_bAccAction  = true;
		return true;
	}

	return false;
}

void BackgroundBoard::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bGameOver || !m_curTetromino)
		return ;

	CCPoint localtion = pTouch->getLocation();

	if(m_bAccAction && pTouch->getDelta().y<-m_actSensitivity)
	{
		gotoTargetPos();
		m_bAccAction = false;
		return ;
	}
	
	float correct = TouchCorrect[m_curTetromino->getShape()][m_curTetromino->getRotate()];
	float targetX = this->convertToNodeSpace(localtion).x;
	targetX += correct*m_blockSize;
	float posX    = -m_curTetromino->getCol() * m_blockSize;

	if(posX < targetX)
	{
		while(posX+m_blockSize < targetX)
		{
			if(!m_curTetromino->move(false,m_bgInfo))
				break;

			posX+=m_blockSize;
		}

		m_curTetromino->setPositionX(posX);
		setTargetBlockPos();
		m_bAccAction = false;
	}
	else
	{
		while(posX-m_blockSize > targetX)
		{
			if(!m_curTetromino->move(true,m_bgInfo))
				break;

			posX-=m_blockSize;
		}

		m_curTetromino->setPositionX(posX);
		setTargetBlockPos();
		m_bAccAction = false;
	}
}

void BackgroundBoard::gotoTargetPos()
{
	int col = m_curTetromino->getCol();
	int row = m_curTetromino->gotoTargetPos();

	m_curTetromino->stopAllActions();
	m_curTetromino->setPosition(ccp(-m_blockSize*col,m_blockSize*row));

	if(m_targetBlock)
	{
		this->removeChild(m_targetBlock);
		m_targetBlock = 0;
	}

	curTetrominoMove();
}

void BackgroundBoard::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint point = pTouch->getLocation();
	if(m_bAccAction && 
	   abs(point.x-m_touchPos.x)<m_actSensitivity &&
	   abs(point.y-m_touchPos.y)<m_actSensitivity)
	{

		m_curTetromino->clockwiseRotate(m_bgInfo);
		m_bAccAction = false;
		setTargetBlockPos();
	}
}

void BackgroundBoard::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
}

void BackgroundBoard::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,0,false);
}

void BackgroundBoard::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	m_bAccAction = false;
}

bool BackgroundBoard::addToBg()
{
	int row = m_curTetromino->getRow();
	int col = m_curTetromino->getCol();
	int rotate = m_curTetromino->getRotate();
	int shape  = m_curTetromino->getShape();

	if(row>=BACKGROUND_ROW)
		return false;

	if(m_targetBlock)
	{
		this->removeChild(m_targetBlock);
		m_targetBlock = 0;
	}

	this->removeChild(m_curTetromino);
	m_curTetromino = 0;

	for(int r = 0 ; r<4 ; ++r)
	{
		int checkRow = row+r;
		if(checkRow<BACKGROUND_ROW)
		{
			m_bgInfo[checkRow] |= (TetrominoShape[shape][rotate][r]<<col);

			if(m_blockSprRow[checkRow] == 0)
			{
				m_blockSprRow[checkRow] = CCNode::create();
				m_blockSprRow[checkRow]->setPosition(0.0f,checkRow*m_blockSize);
				this->addChild(m_blockSprRow[checkRow]);
			}

			for(int i = 0 ; i<4 ; ++i)
			{
				if(TetrominoShape[shape][rotate][r] & 1<<i)
				{
					Block* pBlock = Block::create(m_fnBlockTex);
					pBlock->setEffect(TetrominoEffect(shape));
					pBlock->setPosition(ccp((-i-col)*m_blockSize,0.0f));
					pBlock->setBlockSize(m_blockSize);
					m_blockSprRow[checkRow]->addChild(pBlock);
				}
			}
		}
		else if(TetrominoShape[shape][rotate][r]==0)
		{
			clearLine();
			return true;
		}
		else
			return false;
	}

	clearLine();
	return true;
}

int BackgroundBoard::clearLine()
{
	int moveBy = 0;
	int r = 0;
	for(; r<BACKGROUND_ROW ; ++r)
	{
		if(m_bgInfo[r]==BACKGROUND_ROW_CODE)
		{
			while(m_bgInfo[r+moveBy]==BACKGROUND_ROW_CODE)
			{
				m_bgInfo[r+moveBy] = 0;
				this->removeChild(m_blockSprRow[r+moveBy]);
				m_blockSprRow[r+moveBy] = 0;
				moveBy++;
			}

			break;
		}
	}

	for(; r<BACKGROUND_ROW ; ++r)
	{
		if(r+moveBy<BACKGROUND_ROW)
		{
			m_bgInfo[r] = m_bgInfo[r+moveBy];
			m_blockSprRow[r] = m_blockSprRow[r+moveBy];
			if(m_blockSprRow[r]!=0)
				m_blockSprRow[r]->runAction(CCMoveBy::create(0.2f,ccp(0.0f,-moveBy*m_blockSize))); 
		}
		else
		{
			m_bgInfo[r] = 0;
			m_blockSprRow[r] = 0;
		}
	}

	if(m_clearLineListener && m_clearLineCallback)
	{
		(m_clearLineListener->*m_clearLineCallback)(moveBy);
	}

	return moveBy;
}

void BackgroundBoard::start()
{
	this->removeAllChildren();

	memset(m_bgInfo,0,BACKGROUND_ROW*sizeof(int));

	m_bGameOver		 = false;
	m_bAccAction     = false;

	for(int i = 0 ; i<BACKGROUND_ROW ; ++i)
		m_blockSprRow[i] = 0;

	addNewTetromino();
}

void BackgroundBoard::onGameOver()
{
	if(m_gameOverListener && m_gameOverCallback)
	{
		(m_gameOverListener->*m_gameOverCallback)();
	}
}

void BackgroundBoard::pasueDrop()
{
	if(m_curTetromino)
	{
		m_curTetromino->pauseSchedulerAndActions();
		m_isPause = true;
	}
}

void BackgroundBoard::continueDrop()
{
	if(m_curTetromino)
	{
		m_curTetromino->resumeSchedulerAndActions();
		m_isPause = false;
	}
}