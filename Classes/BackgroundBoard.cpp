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
	if(!CCNode::init())
		return false;

	memset(m_bgInfo,0,BACKGROUND_ROW*sizeof(int));

	this->setAnchorPoint(ccp(1.0f,0.0f));

	m_blockSize = blockSize;
	m_curTetromino = Tetromino::create(rand()%7,m_blockSize,"CloseNormal.png");

	int col = m_curTetromino->getCol();
	int row = m_curTetromino->getRow();
	
	m_curTetromino->setPosition(ccp(-col*m_blockSize,row*m_blockSize));

	CCSequence* pSqe = CCSequence::create(CCMoveBy::create(1.0f,ccp(0.0f,-m_blockSize)), 
										  CCCallFuncN::create(this,callfuncN_selector(BackgroundBoard::curTetrominoMove)),NULL);

	m_curTetromino->runAction(pSqe);
	this->addChild(m_curTetromino);
	return true;
}

void BackgroundBoard::curTetrominoMove(CCNode*)
{

}