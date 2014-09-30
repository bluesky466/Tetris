#include "Tetromino.h"

const int TetrominoShape[][4][4]=
{
	{{0x3,0x3,0x0,0x0},{0x3,0x3,0x0,0x0},{0x3,0x3,0x0,0x0},{0x3,0x3,0x0,0x0}}, //方形
	{{0x2,0x7,0x0,0x0},{0x1,0x3,0x1,0x0},{0x7,0x2,0x0,0x0},{0x2,0x3,0x2,0x0}}, //T型
	{{0x1,0x1,0x1,0x1},{0xf,0x0,0x0,0x0},{0x1,0x1,0x1,0x1},{0xf,0x0,0x0,0x0}}, //1型
	{{0x3,0x2,0x2,0x0},{0x4,0x7,0x0,0x0},{0x1,0x1,0x3,0x0},{0x7,0x1,0x0,0x0}}, //L形
	{{0x3,0x1,0x1,0x0},{0x7,0x4,0x0,0x0},{0x2,0x2,0x3,0x0},{0x1,0x7,0x0,0x0}}, //反L形
	{{0x3,0x6,0x0,0x0},{0x2,0x3,0x1,0x0},{0x3,0x6,0x0,0x0},{0x2,0x3,0x1,0x0}}, //Z形
	{{0x6,0x3,0x0,0x0},{0x1,0x3,0x2,0x0},{0x6,0x3,0x0,0x0},{0x1,0x3,0x2,0x0}}, //反Z形
};

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
		m_blockSprite[i] = CCSprite::create(fnBlockTexture);

		if(m_blockSprite[i] != 0)
		{
			CCSize size = m_blockSprite[i]->getContentSize();
			m_blockSprite[i]->setScaleX(blockSize/size.width);
			m_blockSprite[i]->setScaleY(blockSize/size.height);

			m_blockSprite[i]->setAnchorPoint(ccp(1.0f,0.0f));

			this->addChild(m_blockSprite[i]);
		}
		else
			return false;
	}

	setBlockSprPos();

	return true;
}

bool Tetromino::clockwiseRotate(const int* bgInfo)
{
	int rotateTest = m_rotate+1;
	rotateTest%=4;

	if(!isCollision(m_col,m_row,rotateTest,bgInfo))
	{
		m_rotate = rotateTest;
		setBlockSprPos();
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

bool Tetromino::move(bool bLeft,const int* bgInfo)
{
	int colTest = bLeft ? m_col+1 : m_col-1;
	
	if(!isCollision(colTest,m_row,m_rotate,bgInfo))
	{
		m_col = colTest;
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

bool Tetromino::isCollision(int col,int row,int rotate,const int* bgInfo)
{
	if(row<0 || col<0)
		return true;

	int mask = BACKGROUND_ROW_MASK;

	for(int r = 0 ;r<4 ; ++r)
	{
		if(row+r<BACKGROUND_ROW)
		{
			//左边出界
			if((TetrominoShape[m_shape][rotate][r]<<col) & mask)
				return true;

			//和原来的方块重叠
			if(bgInfo[row+r] & (TetrominoShape[m_shape][rotate][r]<<col))
				return true;
		}
	}

	return false;
}

void Tetromino::addToBg(int* bgInfo)
{
	for(int r = 0 ; r<4 ; ++r)
		bgInfo[m_row+r] |= (TetrominoShape[m_shape][m_rotate][r]<<m_col);
}

bool Tetromino::setCol(int c,int* bgInfo) 
{
	if(!isCollision(c,m_row,m_rotate,bgInfo))
	{
		m_col = c;
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