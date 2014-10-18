#include "Tetromino.h"

Tetromino* Tetromino::create(int shape,float blockSize,const int* bgInfo,Node* parent,bool bCreateTarget)
{
	Tetromino* t = new Tetromino;
	if(t && t->init(shape, blockSize, bgInfo, parent, bCreateTarget))
		return t;

	return 0;
}

void Tetromino::destory(Tetromino** pTetromino)
{
	if(*pTetromino)
	{
		(*pTetromino)->removeNodes();

		delete (*pTetromino);

		(*pTetromino) = 0;
	}
}

Tetromino::Tetromino():
	m_curBlockNode(0),
	m_targetBlockNode(0)
{
}

bool Tetromino::init(int shape,float blockSize,const int* bgInfo,Node* parent,bool bCreateTarget)
{
	m_shape     = shape;
	m_blockSize = blockSize;
	m_rotate    = 0;
	m_col       = 0;
	m_row       = BACKGROUND_ROW;
	
	m_curBlockNode = Node::create();

	if(m_curBlockNode)
		parent->addChild(m_curBlockNode);
	else
		return false;

	for(int i = 0 ; i<4 ; ++i)
	{
		m_curBlockSprites[i] = createBlockSprite(m_shape);

		if(m_curBlockSprites[i]!=0)
			m_curBlockNode->addChild(m_curBlockSprites[i]);
		
		else
		{
			m_curBlockNode->removeFromParentAndCleanup(true);
			m_curBlockNode = 0;
			return false;
		}
	}

	setBlockSprPos(m_curBlockSprites);

	if(bCreateTarget && !createTargetBlockNode(bgInfo,parent))
	{
		m_curBlockNode->removeFromParentAndCleanup(true);
		m_curBlockNode = 0;
		return false;
	}

	m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

	return true;
}

void Tetromino::removeNodes()
{
	if(m_curBlockNode)
	{
		m_curBlockNode->removeFromParentAndCleanup(true);
		m_curBlockNode = 0;
	}

	if(m_targetBlockNode)
	{
		m_targetBlockNode->removeFromParentAndCleanup(true);
		m_targetBlockNode = 0;
	}
}

bool Tetromino::clockwiseRotate(const int* bgInfo)
{
	int colTest    = m_col+g_rotateCorrect[m_shape][m_rotate];
	int rotateTest = m_rotate+1;
	rotateTest%=4;

	if(!isCollision(colTest,m_row,rotateTest,bgInfo))
	{
		m_rotate = rotateTest;
		m_col    = colTest;

		setBlockSprPos(m_curBlockSprites);
		m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

		if(m_targetBlockNode)
		{
			setBlockSprPos(m_targetBlockSprites);
			computeTargetNodePos(bgInfo);
		}

		return true;
	}
	else
	{
		for(int r = 0 ; r<3 ; ++r)
		{
			for(int c = 1 ; c<4 ; ++c)
			{
				if(!isCollision(colTest-c,m_row+r,rotateTest,bgInfo))
				{
					m_col = colTest - c;
					m_row += r;
					m_rotate = rotateTest;

					setBlockSprPos(m_curBlockSprites);
					m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

					if(m_targetBlockNode)
					{
						setBlockSprPos(m_targetBlockSprites);
						computeTargetNodePos(bgInfo);
					}

					return true;
				}
				else if(!isCollision(colTest+c,m_row+r,rotateTest,bgInfo))
				{
					m_col = colTest + c;
					m_row += r;
					m_rotate = rotateTest;

					setBlockSprPos(m_curBlockSprites);
					m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

					if(m_targetBlockNode)
					{
						setBlockSprPos(m_targetBlockSprites);
						computeTargetNodePos(bgInfo);
					}

					return true;
				}
			}
		}
	}

	return false;
}

bool Tetromino::move(bool bLeft,const int* bgInfo)
{
	int colTest = bLeft ? m_col+1 : m_col-1;
	
	if(!isCollision(colTest,m_row,m_rotate,bgInfo))
	{
		m_col = colTest;
		
		m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

		if(m_targetBlockNode)
			computeTargetNodePos(bgInfo);

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

		m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

		return true;
	}

	return false;
}

bool Tetromino::setCol(int c,const int* bgInfo) 
{
	if(!isCollision(c,m_row,m_rotate,bgInfo))
	{
		m_col = c;

		m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

		if(m_targetBlockNode)
			computeTargetNodePos(bgInfo);

		return true;
	}

	return false;
}

bool Tetromino::setRow(int r,const int* bgInfo) 
{
	if(!isCollision(m_col,r,m_rotate,bgInfo))
	{
		m_row = r;

		m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));

		if(m_targetBlockNode)
			computeTargetNodePos(bgInfo);

		return true;
	}

	return false;
}

int Tetromino::gotoTargetPos(const int* bgInfo)
{
	computeTargetNodePos(bgInfo);

	m_row = m_targetRow;
	m_curBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_row));
	
	return m_row;
}

bool Tetromino::createTargetBlockNode(const int* bgInfo,Node* parent)
{
	if(m_targetBlockNode)
	{
		m_targetBlockNode->removeFromParentAndCleanup(true);
		m_targetBlockNode = 0;
	}
	
	m_targetBlockNode = Node::create();

	if(m_targetBlockNode)
		parent->addChild(m_targetBlockNode);
	else
		return false;

	for(int i = 0 ; i<4 ; ++i)
	{
		m_targetBlockSprites[i] = createBlockSprite(m_shape);

		if(m_targetBlockSprites[i]!=0)
			m_targetBlockNode->addChild(m_targetBlockSprites[i]);
		
		else
		{
			m_targetBlockNode->removeFromParentAndCleanup(true);
			m_targetBlockNode = 0;
			return false;
		}
	}

	setBlockSprPos(m_targetBlockSprites);
	computeTargetNodePos(bgInfo);

	return true;
}


bool Tetromino::isCollision(int col,int row,int rotate,const int* bgInfo)
{
	if(row<0 || col<0)
		return true;

	for(int r = 3 ;r>=0 ; --r)
	{
		int code = g_tetrominoShapeCode[m_shape][rotate][r]<<col;

		//左边出界
		if(code & BACKGROUND_ROW_MASK)
		{
			return true;
		}

		//和原来的方块重叠
		if(row+r<BACKGROUND_ROW && (bgInfo[row+r] & code))
		{
			return true;
		}
	}

	return false;
}

bool Tetromino::isTouch(Point worldCoord)
{
	for(int i = 0 ; i<4 ; ++i)
	{
		Size size = m_curBlockSprites[i]->getContentSize();
		Point pos = m_curBlockSprites[i]->convertToNodeSpace(worldCoord);

		if(pos.x>0.0f && pos.y>0.0f && pos.x<size.width && pos.y<size.height)
			return true;
	}

	return false;
}

bool Tetromino::isUnderTetromino(Point worldCoord)
{
	for(int i = 0 ; i<4 ; ++i)
	{
		Size size = m_curBlockSprites[i]->getContentSize();
		Point pos = m_curBlockSprites[i]->convertToNodeSpace(worldCoord);

		if(pos.x>0.0f && pos.x<size.width)
			return true;
	}

	return false;
}

bool Tetromino::isBesideTetromino(Point worldCoord)
{
	for(int i = 0 ; i<4 ; ++i)
	{
		Size size = m_curBlockSprites[i]->getContentSize();
		Point pos = m_curBlockSprites[i]->convertToNodeSpace(worldCoord);

		if(pos.y>0.0f && pos.x<size.height)
			return true;
	}

	return false;
}

TetrominoSize Tetromino::getTetrominoSize()
{
	TetrominoSize size = {0,0};

	for(int r = 0 ; r<4 ; ++r)
		if(g_tetrominoShapeCode[m_shape][m_rotate][r]!=0)
		{
			size._row = r+1;
		}

	for(int c = 0 ;c<4 ; ++c)
	{
		bool flag = false;

		for(int r = 0 ; r<4 ; ++r)
		{
			if(g_tetrominoShapeCode[m_shape][m_rotate][r] >= (1<<c))
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

void Tetromino::setBlockSprPos(Sprite* sprites[4])
{
	int indexBlock = 0;
	for(int r = 0 ; r<4 ; ++r)
		for(int c = 0 ; c<4 ; ++c)
		{
			if(g_tetrominoShapeCode[m_shape][m_rotate][r]&(1<<c))
			{
				Point pos(-c*m_blockSize,r*m_blockSize);
				sprites[indexBlock++]->setPosition(pos);
			}
		}
}

Sprite* Tetromino::createBlockSprite(int shape)
{
	Sprite *block = Sprite::create(g_fnBlockText[shape]);

	if(block)
	{
		block->setAnchorPoint(Vec2(1.0f,0.0f));
		Size sprSize = block->getContentSize();
		block->setScaleX(m_blockSize / sprSize.width);
		block->setScaleY(m_blockSize / sprSize.height);
	}

	return block;
}

void Tetromino::computeTargetNodePos(const int* bgInfo)
{
	m_targetRow = m_row;
	int rowTest = m_targetRow-1;
	
	while(!isCollision(m_col,rowTest,m_rotate,bgInfo))
	{
		m_targetRow = rowTest;
		rowTest--;
	}

	if(m_targetBlockNode)
		m_targetBlockNode->setPosition(Vec2(-m_blockSize*m_col,m_blockSize*m_targetRow));
}