#include "Tetromino.h"

Tetromino::~Tetromino()
{
}

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

	int ret = isCollision(m_col,m_row,rotateTest,bgInfo);
	if(ret==0)
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

	int cloTest = m_col;
	cloTest-=ret;

	//左边碰到了往右移动
	if(0==isCollision(cloTest,m_row,rotateTest,bgInfo))
	{
		m_col    = cloTest;
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

	cloTest = m_col;
	cloTest+= ret;

	//右边碰到了往左移动
	if(0==isCollision(cloTest,m_row,rotateTest,bgInfo))
	{
		m_col    = cloTest;
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
//
//CCNode* Tetromino::getTargetBlockNode(const int* bgInfo,const char* fnBlockTexture)
//{
//	if(!m_targetBlockNode)
//	{
//		m_targetBlockNode = CCNode::create();
//
//		for(int i = 0 ; i<4 ; ++i)
//		{
//			m_targetBlockSprite[i] = Block::create(fnBlockTexture);
//
//			if(m_targetBlockSprite[i]!=0)
//			{
//				m_targetBlockSprite[i]->setBlockSize(m_blockSize);
//				m_targetBlockSprite[i]->setEffect(TargetTetrominoEffect(m_shape));
//				m_targetBlockNode->addChild(m_targetBlockSprite[i]);
//			}
//			else
//				return 0;
//		}
//
//		setTargetBlockSprPos();
//
//		m_targetRow = m_row;
//		while(testDrop(bgInfo))
//		{
//			;//空着就好
//		}
//	}
//
//	return m_targetBlockNode;
//}

bool Tetromino::createTargetBlockNode(const int* bgInfo,const char* fnBlockTexture,CCNode* parent)
{
	if(m_targetBlockNode)
		m_targetBlockNode->removeFromParentAndCleanup(true);
	
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
			return false;
	}

	setTargetBlockSprPos();

	m_targetRow = m_row;
	while(testDrop(bgInfo))
	{
		;//空着就好
	}
	

	parent->addChild(m_targetBlockNode);

	return true;
}

void Tetromino::removeTargetBlockNode()
{
	if(m_targetBlockNode)
	{
		m_targetBlockNode->removeFromParentAndCleanup(true);
		m_targetBlockNode = 0;
	}
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

int Tetromino::isCollision(int col,int row,int rotate,const int* bgInfo)
{
	if(row<0 || col<0)
		return true;

	for(int r = 0 ;r<4 ; ++r)
	{
		int code = TetrominoShape[m_shape][rotate][r]<<col;

		//左边出界
		if(code & BACKGROUND_ROW_MASK)
		{
			//截取出界那几个方块
			code&=BACKGROUND_ROW_MASK;

			//将那几个出界的方块右移到最右边
			code>>=BACKGROUND_COL;

			//看看究竟有多少方块出界了
			if(code >3)
				return 3;
			else if(code >1)
				return 2;
			else
				return 1;
		}

		//和原来的方块重叠
		if(row+r<BACKGROUND_ROW && (bgInfo[row+r] & code))
		{
			//截取碰撞的那几个方块
			code&=bgInfo[row+r];

			//将那几个碰撞的方块右移到最右边
			code>>=col;

			//看看究竟有多少方块碰撞了
			int num = (code&1) + ((code>>1)&1) + ((code>>2)&1) + ((code>>3)&1);
			
			return num;
		}
	}

	return 0;
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

TetrominoSize Tetromino::getTetrominoSize()
{
	TetrominoSize size = {0,0};

	for(int r = 0 ; r<4 ; ++r)
		if(TetrominoShape[m_shape][m_rotate][r]!=0)
		{
			size._row = r+1;
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
			size._col = c+1;
	}

	return size;
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