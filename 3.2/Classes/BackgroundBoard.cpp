#include "BackgroundBoard.h"

BackgroundBoard* BackgroundBoard::create(float blockSize)
{
	BackgroundBoard *pRet = new BackgroundBoard(); 

    if (pRet && pRet->init(blockSize)) 
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

bool BackgroundBoard::init(float blockSize)
{
	if(!Node::init())
		return false;

	memset(m_bgInfo,0,BACKGROUND_ROW*sizeof(int));

	srand(time(0));
	
	m_isPause	        = false;
	m_blockSize         = blockSize;
	m_dropDelayTime     = 0.3f;
	m_tetromino         = 0;
	m_clearLineListener = 0;
	m_clearLineCallback = 0;
	m_gameOverListener  = 0;
	m_gameOverCallback  = 0;
	m_nextBlockListener = 0;
	m_nextBlockCallback = 0;

	m_next3Blocks.clear();
	m_next3Blocks.push_back(rand()%7);
	m_next3Blocks.push_back(rand()%7);
	m_next3Blocks.push_back(rand()%7);

	
	//ÉèÖÃ»Øµ÷
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan     = CC_CALLBACK_2(BackgroundBoard::onTouchBegan,this);
	listener->onTouchMoved     = CC_CALLBACK_2(BackgroundBoard::onTouchMoved,this);
	listener->onTouchCancelled = CC_CALLBACK_2(BackgroundBoard::onTouchCancelled,this);
	listener->onTouchEnded     = CC_CALLBACK_2(BackgroundBoard::onTouchEnded,this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);

	return true;
}

void BackgroundBoard::addNewTetromino()
{
	if(m_tetromino)
		Tetromino::destory(&m_tetromino);

	m_tetromino = Tetromino::create(m_next3Blocks.front(),m_blockSize,m_bgInfo,this,false);
	m_next3Blocks.pop_front();
	m_next3Blocks.push_back(rand()%7);

	int next3Blocks[3];
	std::list<int>::iterator i   = m_next3Blocks.begin();
	std::list<int>::iterator end = m_next3Blocks.end();
	int index = 0;
	while(i!=end)
	{
		next3Blocks[index++] = *i;
		i++;
	}

	if(m_nextBlockListener && m_nextBlockCallback)
		(m_nextBlockListener->*m_nextBlockCallback)(next3Blocks);


	if(m_tetromino->drop(m_bgInfo))
	{
		Sequence* pSqe = Sequence::create(DelayTime::create(m_dropDelayTime),
									      CallFunc::create(CC_CALLBACK_0(BackgroundBoard::tetrominoMove,this)),
										  NULL);
		this->runAction(pSqe);
	}
	else
	{
		Tetromino::destory(&m_tetromino);
		onGameOver();
	}
}

void BackgroundBoard::tetrominoMove()
{
	if(m_tetromino->drop(m_bgInfo))
	{
		Sequence* pSqe = Sequence::create(DelayTime::create(m_dropDelayTime),
										  CallFunc::create(CC_CALLBACK_0(BackgroundBoard::tetrominoMove,this)),
										  NULL);

		this->runAction(pSqe);
	}

	else
	{
		Sequence* pSeq = Sequence::create(DelayTime::create(0.5f),
									      CallFunc::create(CC_CALLBACK_0(BackgroundBoard::setNextTetromino,this)),
										  NULL);
		this->runAction(pSeq);
		
	}
}

void BackgroundBoard::setNextTetromino()
{
	if(m_tetromino == 0)
		return;

	if(m_tetromino->drop(m_bgInfo))
	{
		Sequence* pSqe = Sequence::create(DelayTime::create(m_dropDelayTime),
										  CallFunc::create(CC_CALLBACK_0(BackgroundBoard::tetrominoMove,this)),
										  NULL);

		this->runAction(pSqe);
	}
	
	else if(!addToBg())
		onGameOver();

	else
		addNewTetromino();
			
}

bool BackgroundBoard::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	if(m_bGameOver || m_isPause || !m_tetromino)
		return false;

	Point point = this->convertToNodeSpace(pTouch->getLocation());

	if(point.x>-12*m_blockSize && point.x<0.0f &&  
	   point.y>0.0f && point.y<20.0f*m_blockSize)
	{
		m_touchPos  = this->convertToNodeSpace(pTouch->getLocation());
		m_bAccAction = true;
		m_bAccMove   = false;
		return true;
	}

	return false;
}

void BackgroundBoard::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	if(m_bGameOver || !m_tetromino)
		return ;

	Point localtion = this->convertToNodeSpace(pTouch->getLocation());
	
	if(m_bAccAction && m_touchPos.y - localtion.y>m_blockSize*4)
	{
		gotoTargetPos();
		m_bAccAction = false;
		return ;
	}
	
	float correct = g_touchCorrect[m_tetromino->getShape()][m_tetromino->getRotate()];
	float targetX = localtion.x;
	targetX += correct*m_blockSize;
	float posX    = -m_tetromino->getCol() * m_blockSize;

	if(abs(localtion.x-m_touchPos.x)>m_blockSize)
		m_bAccMove = true;
	
	if(m_bAccMove)
	{
		if(posX < targetX)
		{
			while(posX+m_blockSize < targetX)
			{
				if(!m_tetromino->move(false,m_bgInfo))
					break;

				posX+=m_blockSize;
				m_bAccAction = false;
			}
		}
		else
		{
			while(posX-m_blockSize > targetX)
			{
				if(!m_tetromino->move(true,m_bgInfo))
					break;

				posX-=m_blockSize;
				m_bAccAction = false;
			}
		}
	}
	
}

void BackgroundBoard::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Point point = this->convertToNodeSpace(pTouch->getLocation());
	if(m_bAccAction && 
	   abs(point.x-m_touchPos.x)<m_blockSize &&
	   abs(point.y-m_touchPos.y)<m_blockSize)
	{
		m_tetromino->clockwiseRotate(m_bgInfo);
		m_bAccAction = false;
	}
}

void BackgroundBoard::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
}

void BackgroundBoard::gotoTargetPos()
{
	int row = m_tetromino->gotoTargetPos(m_bgInfo);
}

void BackgroundBoard::onEnter()
{
	CCNode::onEnter();
}

void BackgroundBoard::onExit()
{
	CCNode::onExit();
}

bool BackgroundBoard::addToBg()
{
	int row    = m_tetromino->getRow();
	int col    = m_tetromino->getCol();
	int rotate = m_tetromino->getRotate();
	int shape  = m_tetromino->getShape();

	if(row>=BACKGROUND_ROW)
		return false;

	Tetromino::destory(&m_tetromino);

	for(int r = 0 ; r<4 ; ++r)
	{
		int checkRow = row+r;
		if(checkRow<BACKGROUND_ROW)
		{
			m_bgInfo[checkRow] |= (g_tetrominoShapeCode[shape][rotate][r]<<col);

			if(m_blockSprRow[checkRow] == 0)
			{
				m_blockSprRow[checkRow] = Node::create();
				m_blockSprRow[checkRow]->setPosition(0.0f,checkRow*m_blockSize);
				this->addChild(m_blockSprRow[checkRow]);
			}

			for(int i = 0 ; i<4 ; ++i)
			{
				if(g_tetrominoShapeCode[shape][rotate][r] & 1<<i)
				{
					Sprite *block = Sprite::create(g_fnBlockText[shape]);

					if(block)
					{
						block->setAnchorPoint(Vec2(1.0f,0.0f));
						Size sprSize = block->getContentSize();
						block->setScaleX(m_blockSize / sprSize.width);
						block->setScaleY(m_blockSize / sprSize.height);
						block->setPosition(Vec2((-i-col)*m_blockSize,0.0f));
						m_blockSprRow[checkRow]->addChild(block);
					}
					else
					{
						return false;
					}
				}
			}
		}
		else if(g_tetrominoShapeCode[shape][rotate][r]==0)
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
				m_blockSprRow[r]->runAction(MoveBy::create(0.2f,Vec2(0.0f,-moveBy*m_blockSize))); 
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
	if(m_tetromino)
	{
		m_isPause = true;
		this->pause();
	}
}

void BackgroundBoard::continueDrop()
{
	if(m_tetromino)
	{
		m_isPause = false;
		this->resume();
	}
}