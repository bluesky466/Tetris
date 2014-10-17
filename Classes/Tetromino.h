#ifndef  _TETROMINO_H_
#define  _TETROMINO_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Block.h"

#define TetrominoEffect(n) (ES_USER_DEFINED+n)

#define TargetTetrominoEffect(n) (ES_USER_DEFINED+n+7)

USING_NS_CC;

struct TetrominoSize
{
	int _row;
	int _col;
};

/**
 * 七个形状的方块,包括当前位置的精灵和最后降落的位置的提示精灵
 * 注意：1、原点在右下角,数组的第零行在最底下
 *       2、最终落点的提示精灵需要用 getTargetBlockNode 获得，并用 getTargetRow 获得其行数
 */
class Tetromino : public CCNode
{
public:
	Tetromino();
	~Tetromino();
	bool clockwiseRotate(const int* bgInfo);  ///<顺时针旋转
	bool move(bool bLeft,const int* bgInfo);  ///<左右移动
	bool drop(const int* bgInfo);             ///<下降

	bool setCol(int c,int* bgInfo);
	bool setRow(int r,int* bgInfo);

	int getCol()       {return m_col;}
	int getRow()       {return m_row;}
	int getRotate()    {return m_rotate;}
	int getShape()     {return m_shape;}

	///获取最后降落的行（可以降落的最底下的一行）,列和旋转是和当前的一样的
	int getTargetRow() {return m_targetRow;}  

	///直接降落到可以降落的最底下的一行
	int gotoTargetPos();

	///得到大小
	TetrominoSize getTetrominoSize();

	///判断是否点击到方块上
	bool isTouch(CCPoint worldCoord);

	///判断触点是否在方块的正下方
	bool isUnderTetromino(CCPoint worldCoord);

	///判断触点是否在方块的左右
	bool isBesideTetromino(CCPoint worldCoord);
	
	///得到最终降落位置的提示方块的精灵
	//CCNode* getTargetBlockNode(const int* bgInfo,const char* fnBlockTexture);
	///得到最终降落位置的提示方块的精灵
	bool createTargetBlockNode(const int* bgInfo,const char* fnBlockTexture,CCNode* parent);
	void removeTargetBlockNode();

	void setTargetNodePos(const CCPoint& pos){m_targetBlockNode->setPosition(pos);}

	bool init(int shape,float blockSize,const char* fnBlockTexture);

	static Tetromino* create(int shape,float blockSize,const char* fnBlockTexture);

protected:
	int   m_col;        ///<方块当前位置的列
	int   m_row;        ///<方块当前位置的行
	int   m_targetRow;  ///<方块最后降落的行（可以降落的最底下的一行）
	int   m_rotate;     ///<方块顺时针旋转的次数
	int   m_shape;      ///<方块的形状
	float m_blockSize;  ///<方块的大小（边长）

	Block*  m_blockSprite[4];         ///<当前方块位置的显示精灵
	Block*  m_targetBlockSprite[4];   ///<方块最终降落位置的提示精灵
	CCNode* m_targetBlockNode;        ///<方块最终降落位置的提示精灵就挂在他下面

	void setBlockSprPos();            ///<设置方块当前位置的提示精灵的位置(节点坐标系中的)
	void setTargetBlockSprPos();      ///<方块最终降落位置的提示精灵的位置(节点坐标系中的)
	bool testDrop(const int* bgInfo); ///<用于计算方块最终降落位置

	///判断方块是否与底板的其他方块碰撞
	bool isCollision(int col,int row,int rotate,const int* bgInfo);

	bool testRotate(int rotate,const int* bgInfo);
};

inline Tetromino::Tetromino():
m_col(0),
m_row(BACKGROUND_ROW),
m_rotate(0),
m_shape(0),
m_blockSize(0.0f)
{
}

inline int Tetromino::gotoTargetPos()
{
	m_row = m_targetRow;
	return m_row;
}

#endif