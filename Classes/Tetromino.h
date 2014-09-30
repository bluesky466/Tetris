#ifndef  _TETROMINO_H_
#define  _TETROMINO_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Block.h"

USING_NS_CC;
//原点在右下角,数组的第零行在最底下
class Tetromino : public CCNode
{
public:
	Tetromino();

	bool clockwiseRotate(const int* bgInfo);  //顺时针旋转
	bool move(bool bLeft,const int* bgInfo);  //左右移动
	bool drop(const int* bgInfo);             //下降

	bool setCol(int c,int* bgInfo);
	bool setRow(int r,int* bgInfo);

	int getCol()       {return m_col;}
	int getRow()       {return m_row;}
	int getRotate()    {return m_rotate;}
	int getShape()     {return m_shape;}
	
	bool init(int shape,float blockSize,const char* fnBlockTexture);

	static Tetromino* create(int shape,float blockSize,const char* fnBlockTexture);

protected:
	int   m_col;
	int   m_row;
	int   m_rotate;
	int   m_shape;
	float m_blockSize;
	Block* m_blockSprite[4];

	void setBlockSprPos();
	bool isCollision(int col,int row,int rotate,const int* bgInfo);
};

inline Tetromino::Tetromino():
m_col(0),
m_row(BACKGROUND_ROW),
m_rotate(0),
m_shape(0),
m_blockSize(0.0f)
{
}


#endif