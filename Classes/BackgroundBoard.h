#ifndef _BACKGROUND_BOARD_H_
#define _BACKGROUND_BOARD_H_

#include "cocos2d.h"
#include "GlobalDefine.h"
#include "Tetromino.h"

USING_NS_CC;

class BackgroundBoard : public CCNode
{
public:
	bool init(float blockSize);
	void curTetrominoMove(CCNode*);
	static BackgroundBoard* create(float blockSize);
private:
	int m_blockSize;
	int m_bgInfo[BACKGROUND_ROW];
	Tetromino* m_curTetromino;
};

#endif