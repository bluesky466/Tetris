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

void BackgroundBoard::addNewTetromino()
{
	m_curTetromino = Tetromino::create(rand()%7,m_blockSize,m_fnBlockTex);

	int col = m_curTetromino->getCol();
	int row = m_curTetromino->getRow();
	
	m_curTetromino->setPosition(-col*m_blockSize,row*m_blockSize);

	if(m_curTetromino->drop(m_bgInfo))
	{
		col = m_curTetromino->getCol();
		row = m_curTetromino->getRow();
		CCSequence* pSqe = CCSequence::create(CCMoveBy::create(m_dropDur,ccp(0.0f,-m_blockSize)), 
											  CCCallFuncN::create(this,callfuncN_selector(BackgroundBoard::curTetrominoMove)),
											  NULL);

		m_curTetromino->runAction(pSqe);
		this->addChild(m_curTetromino);
	}
}

bool BackgroundBoard::init(float blockSize,char* fnBlockTex)
{
	if(!CCNode::init())
		return false;

	memset(m_bgInfo,0,BACKGROUND_ROW*sizeof(int));

	m_fnBlockTex     = fnBlockTex;
	m_dropDur        = 0.1f;
	m_blockSize      = blockSize;
	m_actionDistance = 100.0f;
	m_bGameOver		 = false;
	m_bAccAction     = false;

	for(int i = 0 ; i<BACKGROUND_ROW ; ++i)
		m_blockSprRow[i] = 0;

	addNewTetromino();

	return true;
}

void BackgroundBoard::curTetrominoMove(CCNode*)
{
	if(m_curTetromino->drop(m_bgInfo))
	{
		int col = m_curTetromino->getCol();
		int row = m_curTetromino->getRow();
		CCSequence* pSqe = CCSequence::create(CCMoveBy::create(m_dropDur,ccp(0.0f,-m_blockSize)), 
											  CCCallFuncN::create(this,callfuncN_selector(BackgroundBoard::curTetrominoMove)),
											  NULL);

		m_curTetromino->runAction(pSqe);
	}
	else
	{
		if(!addToBg())
		{
			CCMessageBox("GameOver","GameOver");
			m_bGameOver = true;
		}
		else
			addNewTetromino();
			
	}

}

bool BackgroundBoard::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bGameOver)
		return false;
	else
	{
		m_touchPosY = pTouch->getLocation().y;
		m_bAccAction   = true;
		return true;
	}
}

void BackgroundBoard::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bGameOver || !m_curTetromino)
		return ;

	CCPoint localtion = pTouch->getLocation();

	if(m_bAccAction && localtion.y-m_touchPosY>m_actionDistance)
	{
		m_curTetromino->clockwiseRotate(m_bgInfo);
		m_bAccAction = false;
		return ;
	}
	else if(m_bAccAction && m_touchPosY - localtion.y>m_actionDistance)
	{
		while(m_curTetromino->drop(m_bgInfo))
		{
			;
		}

		if(!addToBg())
		{
			CCMessageBox("GameOver","GameOver");
			m_bGameOver = true;
		}
		else
			addNewTetromino();
	
		m_bAccAction = false;
		return ;
	}

	CCPoint target = this->convertToNodeSpace(localtion);
	float posX = -m_curTetromino->getCol() * m_blockSize;

	if(posX < target.x)
	{
		while(posX+m_blockSize < target.x)
		{
			if(!m_curTetromino->move(false,m_bgInfo))
				break;

			posX+=m_blockSize;
		}

		m_curTetromino->setPositionX(posX);
	}
	else
	{
		while(posX-m_blockSize > target.x)
		{
			if(!m_curTetromino->move(true,m_bgInfo))
				break;

			posX-=m_blockSize;
		}

		m_curTetromino->setPositionX(posX);
	}
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
					pBlock->setPosition(ccp((-i-col)*m_blockSize,0.0f));
					pBlock->setBlockSize(m_blockSize);

					m_blockSprRow[checkRow]->addChild(pBlock);
				}
			}
		}
		else if(TetrominoShape[shape][rotate][r]==0)
			return true;
		else
			return false;
	}

	return true;
}

bool BackgroundBoard::clearLine()
{
	return false;
}