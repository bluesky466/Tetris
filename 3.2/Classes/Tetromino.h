#ifndef  _TETROMINO_H_
#define  _TETROMINO_H_

#include "cocos2d.h"
#include "GlobalDefine.h"

USING_NS_CC;

struct TetrominoSize
{
	int _row;
	int _col;
};

/**
 * 七个形状的方块类,包括当前位置的精灵和最后降落的位置的提示精灵
 * 注意：1.原点在右下角,数组的第零行在最底下
 *       2.构造函数和析构函数都为私有,必须用静态方法create创建,静态方法destory销毁
 */
class Tetromino
{
public:
	static Tetromino* create(int shape,float blockSize,const int* bgInfo,Node* parent,bool bCreateTarget = true);
	static void destory(Tetromino** pTetromino);

	bool clockwiseRotate(const int* bgInfo);  ///<顺时针旋转
	bool move(bool bLeft,const int* bgInfo);  ///<左右移动
	bool drop(const int* bgInfo);             ///<下降

	bool setCol(int c,const int* bgInfo);
	bool setRow(int r,const int* bgInfo);

	int getCol()       {return m_col;}
	int getRow()       {return m_row;}
	int getRotate()    {return m_rotate;}
	int getShape()     {return m_shape;}
	int getTargetRow() {return m_targetRow;}  

	void setCurBlockNodePos(const Vec2& position)
	{
		m_curBlockNode->setPosition(position);
	}

	void setCurBlockNodeScale(float scale)
	{
		m_curBlockNode->setScale(scale);
	}

	void setCurBlockNodeAction(Action* action)
	{
		m_curBlockNode->runAction(action);
	}

	void setCurBlockNodeVisible(bool bVisible)
	{
		m_curBlockNode->setVisible(bVisible);
	}

	///直接降落到可以降落的最底下的一行
	int gotoTargetPos(const int* bgInfo);

	///得到大小
	TetrominoSize getTetrominoSize();

	///判断是否点击到方块上
	bool isTouch(Point worldCoord);

	///判断触点是否在方块的正下方
	bool isUnderTetromino(Point worldCoord);

	///判断触点是否在方块的左右
	bool isBesideTetromino(Point worldCoord);

private:
	Tetromino();
	~Tetromino(){}

	int   m_shape;      ///<方块的形状
	int   m_col;        ///<方块当前位置的列
	int   m_row;        ///<方块当前位置的行
	int   m_targetRow;  ///<方块最后降落的行（可以降落的最底下的一行）
	int   m_rotate;     ///<方块顺时针旋转的次数
	float m_blockSize;  ///<小正方形block的大小（边长）

	Node*    m_curBlockNode;          ///<当前方块位置的显示精灵就挂在他下面
	Sprite*  m_curBlockSprites[4];    ///<当前方块位置的显示精灵

	Sprite*  m_targetBlockSprites[4]; ///<方块最终降落位置的提示精灵
	Node*    m_targetBlockNode;       ///<方块最终降落位置的提示精灵就挂在他下面

	bool init(int shape,float blockSize,const int* bgInfo,Node* parent,bool bCreateTarget);

	void removeNodes();

	Sprite* createBlockSprite(int shape);

	///设置block精灵的位置(节点坐标系中)
	void setBlockSprPos(Sprite* sprites[4]);      

	///计算方块提示精灵节点的位置（底板坐标系中）
	void computeTargetNodePos(const int* bgInfo);

	///判断方块是否与底板的其他方块碰撞
	bool isCollision(int col,int row,int rotate,const int* bgInfo);

	///创建最终降落位置的提示方块的精灵
	bool createTargetBlockNode(const int* bgInfo,Node* parent);
};

#endif