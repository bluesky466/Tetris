#include "Tetromino.h"

Tetromino* Tetromino::create(int shape,float blockSize,const char* fnBlockTexture)
{
	Tetromino *pRet = new Tetromino(); 

    if (pRet && pRet->init(shape,blockSize,fnBlockTexture)) 
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

bool Tetromino::init(int shape,float blockSize,const char* fnBlockTexture)
{
	if(!CCNode::init())
		return false;

	m_shape     = shape;
	m_rotate    = 0;
	m_blockSize = blockSize;

	for(int i = 0 ; i<4 ; ++i)
	{
		m_blockSprite[i] = Block::create(fnBlockTexture);

		if(m_blockSprite[i]!=0)
		{
			m_blockSprite[i]->setBlockSize(m_blockSize);
			m_blockSprite[i]->setEffect(TetrominoEffect(m_shape));
			this->addChild(m_blockSprite[i]);
		}
		else
			return false;
	}

	setBlockSprPos();
	m_targetBlockNode = 0;

	return true;
}

bool Tetromino::clockwiseRotate(const int* bgInfo)
{
	int rotateTest = m_rotate+1;
	rotateTest%=4;

	if(!isCollision(m_col,m_row,rotateTest,bgInfo))
	{
		m_rotate = rotateTest;

		m_targetRow = m_row;
		while(testDrop(bgInfo))
		{
			;//空着就好
		}

		setBlockSprPos();
		setTargetBlockSprPos();

		return true;
	}
	return false;
}

void Tetromino::setBlockSprPos()
{
	int indexBlock = 0;
	for(int r = 0 ; r<4 ; ++r)
		for(int c = 0 ; c<4 ; ++c)
		{
			if(TetrominoShape[m_shape][m_rotate][r]&(1<<c))
			{
				CCPoint pos(-c*m_blockSize,r*m_blockSize);
				m_blockSprite[indexBlock++]->setPosition(pos);
			}
		}
}

CCNode* Tetromino::getTargetBlockNode(const int* bgInfo,const char* fnBlockTexture)
{
	if(!m_targetBlockNode)
	{
		m_targetBlockNode = CCNode::create();

		for(int i = 0 ; i<4 ; ++i)
		{
			m_targetBlockSprite[i] = Block::create(fnBlockTexture);

			if(m_targetBlockSprite[i]!=0)
			{
				m_targetBlockSprite[i]->setBlockSize(m_blockSize);
				m_targetBlockSprite[i]->setEffect(TargetTetrominoEffect(m_shape));
				m_targetBlockNode->addChild(m_targetBlockSprite[i]);
			}
			else
				return 0;
		}

		setTargetBlockSprPos();

		m_targetRow = m_row;
		while(testDrop(bgInfo))
		{
			;//空着就好
		}
	}

	return m_targetBlockNode;
}

void Tetromino::setTargetBlockSprPos()
{
	if(!m_targetBlockNode)
		return ;

	int indexBlock = 0;
	for(int r = 0 ; r<4 ; ++r)
		for(int c = 0 ; c<4 ; ++c)
		{
			if(TetrominoShape[m_shape][m_rotate][r]&(1<<c))
			{
				CCPoint pos(-c*m_blockSize,r*m_blockSize);
				m_targetBlockSprite[indexBlock++]->setPosition(pos);
			}
		}
}

bool Tetromino::move(bool bLeft,const int* bgInfo)
{
	int colTest = bLeft ? m_col+1 : m_col-1;
	
	if(!isCollision(colTest,m_row,m_rotate,bgInfo))
	{
		m_col = colTest;

		m_targetRow = m_row;
		while(testDrop(bgInfo))
		{
			;//空着就好
		}

		return true;
	}
	return false;
}

bool Tetromino::drop(const int* bgInfo)
{
	int rowTest = m_row-1;
	
	if(!isCollision(m_col,rowTest,m_rotate,bgInfo))
	{
		m_row = rowTest;
		return true;
	}
	return false;
}

bool Tetromino::testDrop(const int* bgInfo)
{
	int rowTest = m_targetRow-1;
	
	if(!isCollision(m_col,rowTest,m_rotate,bgInfo))
	{
		m_targetRow = rowTest;
		return true;
	}
	return false;
}

bool Tetromino::isCollision(int col,int row,int rotate,const int* bgInfo)
{
	if(row<0 || col<0)
		return true;

	int mask = BACKGROUND_ROW_MASK;

	for(int r = 0 ;r<4 ; ++r)
	{
		//左边出界
		if((TetrominoShape[m_shape][rotate][r]<<col) & mask)
			return true;

		//和原来的方块重叠
		if(row+r<BACKGROUND_ROW && (bgInfo[row+r] & (TetrominoShape[m_shape][rotate][r]<<col)))
			return true;
	}

	return false;
}


bool Tetromino::setCol(int c,int* bgInfo) 
{
	if(!isCollision(c,m_row,m_rotate,bgInfo))
	{
		m_col = c;

		m_targetRow = m_row;
		while(testDrop(bgInfo))
		{
			;//空着就好
		}

		return true;
	}

	return false;
}

bool Tetromino::setRow(int r,int* bgInfo) 
{
	if(!isCollision(m_col,r,m_rotate,bgInfo))
	{
		m_row = r;
		return true;
	}

	return false;
}

bool Tetromino::isTouch(CCPoint worldCoord)
{
	for(int i = 0 ; i<4 ; ++i)
	{
		CCSize size = m_blockSprite[i]->getContentSize();
		CCPoint pos = m_blockSprite[i]->convertToNodeSpace(worldCoord);

		if(pos.x>0.0f && pos.y>0.0f && pos.x<size.width && pos.y<size.height)
			return true;
	}

	return false;
}

CCSize Tetromino::getTetrominoSize()
{
	int row = 0;
	int col = 0;

	for(int r = 0 ; r<4 ; ++r)
		if(TetrominoShape[m_shape][m_rotate][r]!=0)
		{
			row = r+1;
		}

	for(int c = 0 ;c<4 ; ++c)
	{
		bool flag = false;

		for(int r = 0 ; r<4 ; ++r)
		{
			if(TetrominoShape[m_shape][m_rotate][r] >= (1<<c))
			{
				flag = true;
				break;
			}
		}

		if(flag)
			col = c+1;
	}

	return ccp(col*m_blockSize,row*m_blockSize);
}

bool Tetromino::isUnderTetromino(CCPoint worldCoord)
{
	for(int i = 0 ; i<4 ; ++i)
	{
		CCSize size = m_blockSprite[i]->getContentSize();
		CCPoint pos = m_blockSprite[i]->convertToNodeSpace(worldCoord);

		if(pos.x>0.0f && pos.x<size.width)
			return true;
	}

	return false;
}

bool Tetromino::isBesideTetromino(CCPoint worldCoord)
{
	for(int i = 0 ; i<4 ; ++i)
	{
		CCSize size = m_blockSprite[i]->getContentSize();
		CCPoint pos = m_blockSprite[i]->convertToNodeSpace(worldCoord);

		if(pos.y>0.0f && pos.x<size.height)
			return true;
	}

	return false;
}